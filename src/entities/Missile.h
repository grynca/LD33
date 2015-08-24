#ifndef MISSILE_H
#define MISSILE_H

using namespace grynca;
#include "Animation.h"

// fw
class MyGame;

class Missile {
public:
    enum State {
        sOk,
        sExploding,
        sDead
    };

    Missile();

    static void initResources(MyGame& game);
    static Entity& create(MyGame& game, const Vec2& pos, const Vec2& dir);

    void update(MyGame& game);
    void explode();

    Transform transform;
    Renderables renderables;
    Speed speed;

    Rect getBound();

    State state;

    static constexpr float missile_speed = 200.f;
    static constexpr float missile_impact_damage = 1000.0f/60;
    static constexpr float missile_explosion_damage = 20.0f/60;
private:
    enum Sprites {
        sprMissile,
        spritesCount
    };

    void createSprites(MyGame& game);
    SpriteRenderable& getSprite(Sprites s);

    Animation current_animation_;

    static struct Resources_ {
        Animation missile_anim_;
        Animation explosion_anim_;
    } resources_;
};

#endif //MISSILE_H
