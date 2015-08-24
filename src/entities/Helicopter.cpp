#include <Layers.h>
#include "geng.h"
#include "Helicopter.h"
#include "../MyGame.h"
#include "../MyAssets.h"
#include "../Layers.h"

// static
Helicopter::Resources_ Helicopter::resources_;

Helicopter::Helicopter() {

}

void Helicopter::initResources(MyGame& game) {
    MyAssets& assets = game.getModule<MyAssets>();
    resources_.heli_ok_reg = assets.sprites_pack.getRegions().at("data/sprites/heli_ok.png");
    resources_.heli_broken_reg = assets.sprites_pack.getRegions().at("data/sprites/heli_broken.png");

    resources_.explosion_anim_.init(assets.sprites_pack, {
            {"data/sprites/explosion1.png", 5},
            {"data/sprites/explosion2.png", 5},
            {"data/sprites/explosion3.png", 7},
            {"data/sprites/explosion4.png", 7},
            {"data/sprites/explosion5.png", 7},
            {"data/sprites/explosion6.png", 7},
            {"data/sprites/explosion7.png", 5},
            {"data/sprites/explosion8.png", 5}
    }, false);
}

Entity& Helicopter::create(MyGame& game, const Vec2& pos) {
//static
    Entity& ent = game.getModule<EntityManager>().addItem();
    ent.setRoles({erRenderable, erMovable});
    Helicopter& me = ent.set<Helicopter>();
    me.transform.setPosition(pos);
    me.health = 100;
    me.state = sOk;
    me.dodging = 0;
    me.shoot_acc = 0.0f;


    me.createSprites(game);
    me.getSprite(sprHeli).init(layerHelicopters, 0, resources_.heli_ok_reg.getTextureRect(), Vec2{100, 45});

    return ent;
}

void Helicopter::update(MyGame& game) {
    Window& w = game.getModule<Window>();

    if (transform.getPosition().getY() > 350) {
        health = 0;
    }

    if (health <= 0) {
        // explode
        if (state != sExploding) {
            state = sExploding;
            current_animation_ = resources_.explosion_anim_;
            transform.setRotation(0);
            transform.setScale({1, 1});
            speed.setLinearSpeed({0, 0});
            speed.setAngularSpeed(0);
            getSprite(sprHeli).setSize({100, 100});
            getSprite(sprHeli).setTextureCoords(current_animation_.getRegion().getTextureRect());
        }
        else {
            if (current_animation_.update()) {
                if (current_animation_.isAtEnd()) {
                    state = sDead;
                }
                getSprite(sprHeli).setTextureCoords(current_animation_.getRegion().getTextureRect());
            }
        }
    }
    else if (health <= 20) {
        health -= 0.25f;
        if (state != sFallingMore) {
            if (state == sOk) {
                startDropping();
            }
            speed.setAngularSpeed(speed.getAngularSpeed()*2);
            speed.setLinearSpeed(speed.getLinearSpeed()*2);
            state = sFallingMore;
        }
    }
    else if (health < 50) {
        if (state != sFalling) {
            startDropping();
            state = sFalling;
        }
    }
    else {
        // normal motion
        if (transform.getPosition().getX() > 0) {
            transform.setScale(Vec2(-1, 1));
            transform.setRotation(-Angle::Pi/6);
        }
        else {
            transform.setRotation(Angle::Pi/6);
        }

        shoot_acc += shoot_rate;
        if (shoot_acc > 1) {

            Vec2 to_monster = game.getMonster().getEyePosition() - transform.getPosition();
            to_monster = normalize(to_monster);
            game.missiles.spawnNew(transform.getPosition()+to_monster*50, to_monster);
            shoot_acc = 0;
        }
    }

}

Rect Helicopter::getBound() {
    SpriteRenderable& sr = getSprite(sprHeli);
    return Rect(transform.getPosition(), sr.getSize(), sr.getOffset(), transform.getRotation());
}

void Helicopter::createSprites(MyGame& game) {
    Window& w = game.getModule<Window>();
    for (uint32_t i=0; i<spritesCount; ++i)
        renderables.add<SpriteRenderable>(w);
}

SpriteRenderable& Helicopter::getSprite(Sprites s) {
    return *((SpriteRenderable*)renderables.get(s));
}

void Helicopter::startDropping() {
    getSprite(sprHeli).setTextureCoords(resources_.heli_broken_reg.getTextureRect());
    speed.setAngularSpeed(randFloat()*Angle::Pi/3-Angle::Pi/6);
    Vec2 drop_dir(randFloat()*2-1, 1);
    speed.setLinearSpeed(drop_dir*drop_speed);
}
