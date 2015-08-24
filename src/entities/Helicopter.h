#ifndef HELICOPTER_H
#define HELICOPTER_H

using namespace grynca;
#include "Animation.h"

// fw
class MyGame;

class Helicopter {
public:
    enum State {
        sOk,
        sFalling,
        sFallingMore,
        sExploding,
        sDead
    };

    Helicopter();

    static void initResources(MyGame& game);
    static Entity& create(MyGame& game, const Vec2& pos);

    void update(MyGame& game);

    Transform transform;
    Renderables renderables;
    Speed speed;

    Rect getBound();

    float health;
    State state;
    uint32_t dodging;
    float shoot_acc;

    static constexpr float heli_speed = 45.f;
    static constexpr float drop_speed = 100.f;
    static constexpr float heli_damage = 50.0f/60;
    static constexpr float shoot_rate = 0.3f/60;
private:
    enum Sprites {
        sprHeli,
        spritesCount
    };

    void createSprites(MyGame& game);
    SpriteRenderable& getSprite(Sprites s);

    void startDropping();

    Animation current_animation_;

    static struct Resources_ {
        TextureRegion heli_ok_reg;
        TextureRegion heli_broken_reg;
        Animation explosion_anim_;
    } resources_;
};

#endif //HELICOPTER_H
