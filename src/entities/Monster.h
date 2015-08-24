#ifndef MONSTER_H
#define MONSTER_H

using namespace grynca;
#include "Animation.h"

// fw
class MyGame;

class Monster {
public:
    enum State {
        sEye,
        sMorphingToEye,
        sMorphingToHand,
        sHand,
        sDead
    };

    Monster();

    static void initResources(Game& game);
    static Entity& create(Game& game);

    void update(MyGame& game);

    Ray getLaserRay();
    Rect getEyeBound();
    Vec2 getEyePosition();

    Transform transform;
    Renderables renderables;
    Speed speed;

    float health;
    float laser;
    State state;
private:
    enum Sprites {
        sprBase,
        sprEye,

        spritesCount
    };

    enum Hud {
        hudHealthBar,
        hudLaserBar,

        hudCount
    };

    void createSprites(Game& game);
    void createNeck(Game& game);
    void createLaser(Game& game);

    SpriteRenderable& getSprite(Sprites s);
    RectRenderable& getNeckPart(uint32_t part_id);
    RectRenderable& getLaser();
    void gameOver(MyGame& game);

    Animation current_animation_;

    static constexpr float eye_travel_stiffness = 0.02f;
    static constexpr float eye_rot_stiffness = 0.05f;
    static constexpr float max_neck_length = 350.f;
    static constexpr float min_dist_to_mouse = 50.f;
    static constexpr uint32_t neck_parts_count = 10;
    static constexpr float neck_stiffness = 0.4f;
    static constexpr float neck_bottom_thickness = 40;
    static constexpr float neck_thickness_decrease = 30/neck_parts_count;
    static constexpr float neck_part_len = max_neck_length/neck_parts_count;
    static constexpr float max_laser_len = 5000;
    static constexpr float laser_dmg = 120.f/60;
    static constexpr float eye_dmg = 200.f/60;
    static constexpr float laser_drain = 30.f/60;
    static constexpr float laser_recharge = 15.f/60;
    static constexpr float dmg_resist_eye = 0.5f;
    static constexpr float dmg_resist_hand = 0.9f;
    static constexpr float final_explosion_speed = 300.f/60;

    static struct Resources_ {
        Animation eye_to_hand_anim_;
        Animation hand_to_eye_anim_;
        TextureRegion base_reg_;
        TextureRegion eye_dead_reg_;
    } resources_;
};

#endif //MONSTER_H
