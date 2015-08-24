#include <Layers.h>
#include "geng.h"
#include "Missile.h"
#include "../MyGame.h"
#include "../MyAssets.h"
#include "../Layers.h"

// static
Missile::Resources_ Missile::resources_;

Missile::Missile() {

}

void Missile::initResources(MyGame& game) {
    MyAssets& assets = game.getModule<MyAssets>();
    resources_.explosion_anim_.init(assets.sprites_pack, {
            {"data/sprites/explosion1.png", 2},
            {"data/sprites/explosion2.png", 2},
            {"data/sprites/explosion3.png", 5},
            {"data/sprites/explosion4.png", 5},
            {"data/sprites/explosion5.png", 5},
            {"data/sprites/explosion6.png", 5},
            {"data/sprites/explosion7.png", 2},
            {"data/sprites/explosion8.png", 2}
    }, false);
    resources_.missile_anim_.init(assets.sprites_pack, {
            {"data/sprites/missile1.png", 10},
            {"data/sprites/missile2.png", 10},
    }, true);
}

Entity& Missile::create(MyGame& game, const Vec2& pos, const Vec2& dir) {
//static
    Entity& ent = game.getModule<EntityManager>().addItem();
    ent.setRoles({erRenderable, erMovable});
    Missile& me = ent.set<Missile>();
    me.transform.setPosition(pos);
    me.transform.setRotation(dir.getAngle());
    me.speed.setLinearSpeed(dir*missile_speed);

    me.createSprites(game);
    me.current_animation_ = resources_.missile_anim_;
    me.getSprite(sprMissile).init(layerMissiles, 0, me.current_animation_.getRegion().getTextureRect(), Vec2{42, 18});

    return ent;
}

void Missile::update(MyGame& game) {

    if (current_animation_.update()) {
        if (state == sExploding && current_animation_.isAtEnd()) {
            state = sDead;
        }
        getSprite(sprMissile).setTextureCoords(current_animation_.getRegion().getTextureRect());
    }

    ViewPort& vp = game.getModule<Window>().getViewPort();
    Vec2 pos_screen = vp.worldToView(transform.getPosition());
    // kill missiles when out of screen
    if (pos_screen.getX() < 0 || pos_screen.getX()>vp.getBaseSize().getX()
        || pos_screen.getY() < 0 || pos_screen.getY()>vp.getBaseSize().getY()) {
        state = sDead;
        return;
    }
}

void Missile::explode() {
    state = Missile::sExploding;
    current_animation_ = resources_.explosion_anim_;
    getSprite(sprMissile).setTextureCoords(current_animation_.getRegion().getTextureRect());
    transform.setPosition(transform.getPosition() + speed.getLinearSpeed()/10); // explode a little further than impacted
    transform.setRotation(0);
    transform.setScale({1, 1});
    speed.setLinearSpeed({0, 0});
    speed.setAngularSpeed(0);
    getSprite(sprMissile).setSize({50, 50});
}

Rect Missile::getBound() {
    SpriteRenderable& sr = getSprite(sprMissile);
    return Rect(transform.getPosition(), sr.getSize(), sr.getOffset(), transform.getRotation());
}

void Missile::createSprites(MyGame& game) {
    Window& w = game.getModule<Window>();
    for (uint32_t i=0; i<spritesCount; ++i)
        renderables.add<SpriteRenderable>(w);
}

SpriteRenderable& Missile::getSprite(Sprites s) {
    return *((SpriteRenderable*)renderables.get(s));
}