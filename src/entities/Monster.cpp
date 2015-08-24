#include "geng.h"
#include "Monster.h"
#include "../MyGame.h"
#include "../MyAssets.h"
#include "../Layers.h"

// static
Monster::Resources_ Monster::resources_;

Monster::Monster() {

}

void Monster::initResources(Game& game) {
    MyAssets& assets = game.getModule<MyAssets>();
    resources_.eye_to_hand_anim_.init(assets.sprites_pack, {
            {"data/sprites/eye_to_hand_1.png", 5},
            {"data/sprites/eye_to_hand_2.png", 5},
            {"data/sprites/eye_to_hand_3.png", 5},
            {"data/sprites/eye_to_hand_4.png", 5},
            {"data/sprites/eye_to_hand_5.png", 5}
    }, false);
    resources_.hand_to_eye_anim_.init(assets.sprites_pack, {
            {"data/sprites/eye_to_hand_5.png", 5},
            {"data/sprites/eye_to_hand_4.png", 5},
            {"data/sprites/eye_to_hand_3.png", 5},
            {"data/sprites/eye_to_hand_2.png", 5},
            {"data/sprites/eye_to_hand_1.png", 5}
    }, false);
    resources_.base_reg_ = assets.sprites_pack.getRegions().at("data/sprites/base.png");
    resources_.eye_dead_reg_ = assets.sprites_pack.getRegions().at("data/sprites/eye_dead.png");
}

Entity& Monster::create(Game& game) {
//static
    Entity& ent = game.getModule<EntityManager>().addItem();
    ent.setRoles({erRenderable, erMovable});
    Monster& me = ent.set<Monster>();
    me.state = sEye;
    me.health = 100;
    me.laser = 100;
    me.transform.setPosition({0, 300});

    me.current_animation_ = resources_.hand_to_eye_anim_;
    me.current_animation_.setCurrentFrame(3);

    me.createSprites(game);
    me.getSprite(sprBase).init(layerMonster, 0, resources_.base_reg_.getTextureRect(), Vec2{100, 100});
    me.getSprite(sprEye).init(layerMonsterEye, 0, me.current_animation_.getRegion().getTextureRect(), Vec2{100, 100});
    me.getSprite(sprEye).getLocalTransform().setPosition(Vec2{0, -max_neck_length});

    me.createNeck(game);
    me.createLaser(game);

    return ent;
}

void Monster::update(MyGame& game) {

    if (game.game_won) {
        return;
    }

    if (state == sDead) {
        // shoot body parts everywhere
        uint32_t parts_count = neck_parts_count+2;
        Angle delta_angle = 2*Angle::Pi/parts_count;
        Angle a = -Angle::Pi/6;

        Transform& et = getSprite(sprEye).getLocalTransform();
        et.setPosition(et.getPosition() + Vec2(1, 0).rotate(a)*final_explosion_speed );
        a += delta_angle;

        for (uint32_t i=0; i<neck_parts_count; ++i) {
            Transform& nt = getNeckPart(i).getLocalTransform();
            nt.setPosition(nt.getPosition() + Vec2(1, 0).rotate(a)*final_explosion_speed );
            a += delta_angle;
        }

        Transform& bt = getSprite(sprBase).getLocalTransform();
        et.setPosition(bt.getPosition() + Vec2(1, 0).rotate(a)*final_explosion_speed );

        return;
    }

    if (health < 0) {
        gameOver(game);
        return;
    }

    Window& w = game.getModule<Window>();
    Events& e = w.getEvents();
    if (current_animation_.update()) {
        if (current_animation_.isAtEnd()) {
            if (state == sMorphingToHand) {
                state = sHand;
            }
            else if (state == sMorphingToEye) {
                state = sEye;
            }
        }
        getSprite(sprEye).setTextureCoords(current_animation_.getRegion().getTextureRect());
    }

    if (e.wasButtonClicked(MouseButton::mbRight)) {
        current_animation_.changeDir();
        state = sMorphingToHand;
    }

    if (e.wasButtonReleased(MouseButton::mbRight)) {
        current_animation_.changeDir();
        state = sMorphingToEye;
    }

    getLaser().setVisible(e.isButtonDown(MouseButton::mbLeft));

    Vec2 mouse_pos = e.getMousePos();
    Vec2 mouse_local = Mat3::invert(transform.getTransform())*w.getViewPort().viewToWorld(mouse_pos);
    //std::cout << "mouse_local: " << mouse_local << std::endl;

    Vec2 eye_pos = transform.getTransform()*getSprite(sprEye).getLocalTransform().getTransform()*Vec2(0,0);
    Vec2 eye_pos_screen = w.getViewPort().worldToView(eye_pos);

    Vec2 to_mouse_screen = mouse_pos - eye_pos_screen;
    if (!to_mouse_screen.isZero()) {
        Transform& eye_t = getSprite(sprEye).getLocalTransform();

        Vec2 eye_pos_local = eye_t.getPosition();
        Vec2 eye_to_mouse = mouse_local - eye_pos_local;
        float d_to_mouse = eye_to_mouse.getLen();
        float coeff = (d_to_mouse-min_dist_to_mouse)/d_to_mouse;
        eye_pos_local += eye_to_mouse*coeff*eye_travel_stiffness;

        float d_to_base = eye_pos_local.getLen();
        if (d_to_base > max_neck_length) {
            eye_pos_local *= max_neck_length/d_to_base;
        }

        eye_t.setPosition(eye_pos_local);

        // look to mouse
        Angle angle_delta = to_mouse_screen.getAngle()-eye_t.getRotation();
        eye_t.setRotation(eye_t.getRotation()+angle_delta*eye_rot_stiffness);

        //RectRenderable& last_part = getNeckPart(neck_parts_count-1);
        Vec2 next_pos = eye_pos_local; // - Vec2(neck_part_len, 0).rotate(eye_t.getRotation());
//        last_part.getLocalTransform().setPosition(next_pos);
//        last_part.getLocalTransform().setRotation(eye_t.getRotation());
        for (int32_t i=neck_parts_count-1; i>=0; --i) {
            RectRenderable& neck_part = getNeckPart(i);
            Transform& t = neck_part.getLocalTransform();
            Vec2 pos = t.getPosition();

            float d_to_base = pos.getLen();
            float desired_dist = i*(float)neck_part_len;;
            if (d_to_base > desired_dist) {
                pos *= desired_dist/d_to_base;
            }

            Vec2 to_next = next_pos - pos;
            float to_next_dist = to_next.getLen();
            float coeff = (to_next_dist-neck_part_len)/to_next_dist;
            pos += to_next*coeff*neck_stiffness;
            t.setPosition(pos);

            t.setRotation(to_next.getAngle());
            next_pos = pos;
        }
        getNeckPart(0).getLocalTransform().setPosition(Vec2(0,0));
    }

    // put laser to eye
    getLaser().getLocalTransform() = getSprite(sprEye).getLocalTransform();

    bool firing = false;
    if (getLaser().getVisible()) {
        if (state == sEye && laser > 0) {
            firing = true;
            // firing laser
            Ray laser_ray = getLaserRay();
            EntityManager& ent_mgr = game.getModule<EntityManager>();
            OverlapInfo oi;
            oi.setDepth(max_laser_len);

            bool target_is_missile = false;
            void* target = NULL;
            for (uint32_t i=0; i<game.helicopters.getHelicoptersCount(); ++i) {
                Helicopter& heli = game.helicopters.getHelicopter(i);
                if (heli.state == Helicopter::sExploding)
                    continue;
                Rect heli_bound = heli.getBound();
                OverlapInfo tmp_oi;
                if (laser_ray.overlaps(heli_bound, tmp_oi)) {
                    if (tmp_oi.getDepth() < oi.getDepth()) {
                        oi = tmp_oi;
                        target = &heli;
                    }
                }
            }

            for (uint32_t i=0; i<game.missiles.getMissilesCount(); ++i) {
                Missile& m = game.missiles.getMissile(i);
                if (m.state == Missile::sExploding)
                    continue;
                Rect m_bound = m.getBound();
                OverlapInfo tmp_oi;
                if (laser_ray.overlaps(m_bound, tmp_oi)) {
                    if (tmp_oi.getDepth() < oi.getDepth()) {
                        oi = tmp_oi;
                        target = &m;
                        target_is_missile = true;
                    }
                }
            }

            getLaser().setSize({oi.getDepth(), 5});
            if (target) {
                if (target_is_missile) {
                    ((Missile*)target)->explode();
                }
                else {
                    ((Helicopter*)target)->health -=laser_dmg;
                }
            }
            laser -= laser_drain;
        }
    }
    if (!firing) {
        getLaser().setVisible(false);
        if (laser < 100){
            laser += laser_recharge;
        }
    }

    // eye crash with helicopters
    Rect eye_bound = getEyeBound();
    for (uint32_t i=0; i<game.helicopters.getHelicoptersCount(); ++i) {
        Helicopter& heli = game.helicopters.getHelicopter(i);
        if (eye_bound.overlaps(heli.getBound())) {
            heli.health -= eye_dmg;
            if (state == sHand) {
                health -= Helicopter::heli_damage*(1-dmg_resist_hand);
            }
            else {
                health -= Helicopter::heli_damage*(1-dmg_resist_eye);
            }
        }
    }
    // eye crash with missiles
    for (uint32_t i=0; i<game.missiles.getMissilesCount(); ++i) {
        Missile& m = game.missiles.getMissile(i);
        if (eye_bound.overlaps(m.getBound())) {
            float dmg;
            if (m.state == Missile::sOk) {
                dmg = Missile::missile_impact_damage;
                m.explode();
            }
            else {
                dmg = Missile::missile_explosion_damage;
            }
            if (state == sHand) {
                health -= dmg*(1-dmg_resist_hand);
            }
            else {
                health -= dmg*(1-dmg_resist_eye);
            }
        }
    }

}

Ray Monster::getLaserRay() {
    RectRenderable& rr = getLaser();
    Vec2 pos = transform.getTransform()*rr.getLocalTransform().getTransform()*Vec2(0,0);
    Angle world_rot = transform.getRotation()+rr.getLocalTransform().getRotation();
    //std::cout << "monster rotation: " << transform.getRotation() << std::endl;
    //std::cout << "laser rotation: " << rr.getLocalTransform().getRotation() << std::endl;
    Vec2 dir = Vec2(1.0f, 0.0f).rotate(world_rot);
    return Ray(pos, dir, max_laser_len);
}

Rect Monster::getEyeBound() {
    SpriteRenderable& sr = getSprite(sprEye);
    Vec2 world_pos = transform.getTransform()*sr.getLocalTransform().getTransform()*Vec2(0,0);
    Angle world_rot = transform.getRotation()+sr.getLocalTransform().getRotation();
    return Rect(world_pos, sr.getSize(), sr.getOffset(), world_rot);
}

Vec2 Monster::getEyePosition() {
    return transform.getTransform()*getSprite(sprEye).getLocalTransform().getTransform()*Vec2(0,0);
}

void Monster::createSprites(Game& game) {
    Window& w = game.getModule<Window>();
    for (uint32_t i=0; i<spritesCount; ++i)
        renderables.add<SpriteRenderable>(w);
}

void Monster::createNeck(Game& game) {
    Window& w = game.getModule<Window>();
    for (uint32_t i=0; i<neck_parts_count; ++i) {
        float dist = i*(float)neck_part_len;
        Vec2 rect_size((float)neck_part_len*1.4f, neck_bottom_thickness - i*neck_thickness_decrease);
        RectRenderable& neck_rect = renderables.add<RectRenderable>(game.getModule<Window>(), layerMonsterNeck, rect_size);
        neck_rect.setColor(0.223, 0.247, 0.01);
        neck_rect.getLocalTransform().setPosition({0, -dist});
        neck_rect.getLocalTransform().setRotation(Angle::Pi/2);
    }
}

void Monster::createLaser(Game& game) {
    RectRenderable& laser_rect = renderables.add<RectRenderable>(game.getModule<Window>(), layerMonster, Vec2(max_laser_len, 5), Vec2(0, -2.5));
    laser_rect.setColor(1, 0, 0, 1);
    laser_rect.setVisible(false);
}

SpriteRenderable& Monster::getSprite(Sprites s) {
    return *((SpriteRenderable*)renderables.get(s));
}

RectRenderable& Monster::getNeckPart(uint32_t part_id) {
    return *((RectRenderable*)renderables.get(spritesCount+part_id));
}

RectRenderable& Monster::getLaser() {
    return *((RectRenderable*)renderables.get(spritesCount+neck_parts_count));
}

void Monster::gameOver(MyGame& game) {
    state = sDead;
    getSprite(sprEye).setTextureCoords(resources_.eye_dead_reg_.getTextureRect());
    getSprite(sprEye).getLocalTransform().setRotation(-Angle::Pi/2);
    game.getHUD().gameOver(game);
}