#ifndef HELISPAWNER_H
#define HELISPAWNER_H

#include "geng.h"
using namespace grynca;

// fw
class MyGame;

class Helicopters {
public:
    void init(MyGame& game);
    void update();
    // where is 0-1
    Entity& spawnNew(uint32_t edge, float where);

    uint32_t getHelicoptersCount()const;
    Helicopter& getHelicopter(uint32_t id);

    void updateHeliMotion(uint32_t heli_id);

    void winGame();
    Vec2 gather_points[6];
private:
    uint32_t frames_from_last_spawn_;
    uint32_t frames_between_spawns_;
    float fastering_factor_;

    static constexpr float heli_aware_dist_sqr = 20000;

    Vec2 findNearestHeli(uint32_t heli_id, float& sqrd_out);
    Vec2 findNearestGatherPoint(uint32_t heli_id, float& sqrd_out);

    fast_vector<VersionedIndex> helicopter_ids_;
    fast_vector<VersionedIndex> delete_next_frame_;
    MyGame* game_;
};

#endif //HELISPAWNER_H
