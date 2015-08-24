#include "Missiles.h"
#include "entities/Missile.h"
#include "MyGame.h"

void Missiles::init(MyGame& game) {
    game_ = &game;
    Missile::initResources(game);
}

void Missiles::update() {
    EntityManager& em = game_->getModule<EntityManager>();
    while (delete_next_frame_.size()) {
        em.removeItem(delete_next_frame_.back());
        delete_next_frame_.pop_back();
    }

    for (uint32_t i=0; i<getMissilesCount(); ++i) {
        Missile& m = getMissile(i);
        m.update(*game_);
        if (m.state == Missile::sDead) {
            delete_next_frame_.push_back(missile_ids_[i]);
            missile_ids_.erase(missile_ids_.begin()+i);
            --i;
            continue;
        }
//        if (heli.state == Helicopter::sOk) {
//            updateHeliMotion(i);
//        }

    }
}

Entity& Missiles::spawnNew(const Vec2& pos, const Vec2& dir) {
    Entity& m = Missile::create(*game_, pos, dir);
    missile_ids_.push_back(m.getId());
    return m;
}

uint32_t Missiles::getMissilesCount()const {
    return uint32_t(missile_ids_.size());
}

Missile& Missiles::getMissile(uint32_t id) {
    return game_->getModule<EntityManager>().getItem(missile_ids_[id]).get<Missile>();
}
