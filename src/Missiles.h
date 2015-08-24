#ifndef MISSILES_H
#define MISSILES_H

#include "geng.h"
using namespace grynca;

// fw
class MyGame;
class Missile;

class Missiles {
public:
    void init(MyGame& game);
    void update();

    Entity& spawnNew(const Vec2& pos, const Vec2& dir);

    uint32_t getMissilesCount()const;
    Missile& getMissile(uint32_t id);
private:
    fast_vector<VersionedIndex> delete_next_frame_;
    fast_vector<VersionedIndex> missile_ids_;
    MyGame* game_;
};

#endif //MISSILES_H
