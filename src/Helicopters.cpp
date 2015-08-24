#include "Helicopters.h"
#include "MyGame.h"
#include "entities/Helicopter.h"

void Helicopters::init(MyGame& game) {
    game_ = &game;
    Helicopter::initResources(game);
    frames_from_last_spawn_ = 0;
    frames_between_spawns_ = 300;
    fastering_factor_ = 0.95;

    ViewPort& vp = game_->getModule<Window>().getViewPort();
    gather_points[0] = Vec2(-300, -200);
    gather_points[1] = Vec2(-400, 0);
    gather_points[2] = Vec2(-300, 200);
    gather_points[3] = Vec2( 300, 200);
    gather_points[4] = Vec2( 400, 0);
    gather_points[5] = Vec2( 300, -200);
}

void Helicopters::update() {

    EntityManager& em = game_->getModule<EntityManager>();
    while (delete_next_frame_.size()) {
        em.removeItem(delete_next_frame_.back());
        delete_next_frame_.pop_back();
    }

    if (!game_->game_won) {
        ++frames_from_last_spawn_;
        if (frames_from_last_spawn_ == frames_between_spawns_) {
            // spawn
            float rand_f = (float)(rand()%RAND_MAX)/RAND_MAX;
            Entity& e = spawnNew(rand()%3, rand_f);
            frames_from_last_spawn_ = 0;
            frames_between_spawns_ *= fastering_factor_;
            if (frames_between_spawns_ < 50) {
                frames_between_spawns_ = 300;
                fastering_factor_ *= 0.8;
            }
        }
    }

    for (uint32_t i=0; i<getHelicoptersCount(); ++i) {
        Helicopter& heli = getHelicopter(i);
        heli.update(*game_);
        if (heli.state == Helicopter::sDead) {
            delete_next_frame_.push_back(helicopter_ids_[i]);
            helicopter_ids_.erase(helicopter_ids_.begin()+i);
            --i;
            continue;
        }
        if (heli.state == Helicopter::sOk) {
            updateHeliMotion(i);
        }

    }

    // heli-heli collisions
    for (uint32_t i=0; i<getHelicoptersCount(); ++i) {
        Helicopter& h1 = getHelicopter(i);
        for (uint32_t j=i+1; j<getHelicoptersCount(); ++j) {
            Helicopter& h2 = getHelicopter(j);
            if (h1.state==Helicopter::sOk || h2.state == Helicopter::sOk) {
                if (h1.getBound().overlaps(h2.getBound())) {
                    h1.health -= Helicopter::heli_damage;
                    h2.health -= Helicopter::heli_damage;
                    break;
                }
            }
        }
    }

}


uint32_t Helicopters::getHelicoptersCount()const {
    return helicopter_ids_.size();
}

Helicopter& Helicopters::getHelicopter(uint32_t id) {
    return game_->getModule<EntityManager>().getItem(helicopter_ids_[id]).get<Helicopter>();
}

Entity& Helicopters::spawnNew(uint32_t edge, float where) {
    ViewPort& vp = game_->getModule<Window>().getViewPort();
    Vec2 screen_spawn_point;
    switch (edge) {
        case 0:
            // left
            screen_spawn_point = Vec2(0, where*vp.getBaseSize().getY());
            break;
        case 1:
            // top
            screen_spawn_point = Vec2(where*vp.getBaseSize().getX(), 20);
            break;
        default:
            // right
            screen_spawn_point = Vec2(vp.getBaseSize().getX(), where*vp.getBaseSize().getY());
            break;
    }
    Vec2 spawn_point = vp.viewToWorld(screen_spawn_point);
    Entity& heli = Helicopter::create(*game_, spawn_point);
    helicopter_ids_.push_back(heli.getId());
    return heli;
}

void Helicopters::winGame() {
    for (uint32_t i=0; i<getHelicoptersCount(); ++i) {
        Helicopter& heli = getHelicopter(i);
        heli.health = 19.9;
    }
}

void Helicopters::updateHeliMotion(uint32_t heli_id) {

    Helicopter& h = getHelicopter(heli_id);
    if (h.dodging) {
        --h.dodging;
    }

    float sqrd;
    Vec2 motion_v = -findNearestHeli(heli_id, sqrd);
    if (sqrd >= heli_aware_dist_sqr) {
        if (!h.dodging) {
            // no need to dodge other heli -> go to gather point
            motion_v = findNearestGatherPoint(heli_id, sqrd);
        }
    }
    else {
        h.dodging = 60;
    }
    h.speed.setLinearSpeed(normalize(motion_v)*Helicopter::heli_speed);


//    for (uint32_t j=heli_id; j<getHelicoptersCount(); ++j) {
//        Helicopter& h2 = getHelicopter(j);
//        if (h1.getBound().overlaps(h2.getBound())) {
//            h1.health = 0;
//            h2.health = 0;
//        }
//    }
}

Vec2 Helicopters::findNearestHeli(uint32_t heli_id, float& sqrd_out) {
    Helicopter& h = getHelicopter(heli_id);

    Vec2 dist_v;
    sqrd_out = std::numeric_limits<float>::max();
    for (uint32_t i=0; i<getHelicoptersCount(); ++i) {
        if (i == heli_id)
            continue;
        Helicopter& h2 = getHelicopter(i);
        Vec2 v = h2.transform.getPosition()-h.transform.getPosition();
        float sqrd = v.getSqrLen();
        if (sqrd < sqrd_out) {
            sqrd_out = sqrd;
            dist_v = v;
        }
    }
    return dist_v;
}

Vec2 Helicopters::findNearestGatherPoint(uint32_t heli_id, float& sqrd_out) {
    Helicopter& h = getHelicopter(heli_id);

    Vec2 dist_v;
    sqrd_out = std::numeric_limits<float>::max();
    for (uint32_t i=0; i<6; ++i) {
        Vec2 v = gather_points[i]-h.transform.getPosition();
        float sqrd = v.getSqrLen();
        if (sqrd < sqrd_out) {
            sqrd_out = sqrd;
            dist_v = v;
        }
    }
    return dist_v;
}