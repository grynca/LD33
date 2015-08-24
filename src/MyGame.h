#ifndef MYGAME_H
#define MYGAME_H

#include "geng.h"
using namespace grynca;
#include "Helicopters.h"
#include "Missiles.h"

// fw
class Monster;
class HUD;

class MyGame : public Game, public Singleton<MyGame> {
public:
    MyGame();


    Helicopters helicopters;
    Missiles missiles;
    Monster& getMonster();
    HUD& getHUD();
    bool game_won;
private:
    VersionedIndex monster_id_;
    VersionedIndex hud_id_;

    virtual void init() override;
    virtual void update() override;
    virtual void tick() override;

    void updateDesktopShortcuts_();
    void updateCameraMotion_();
};




#endif //MYGAME_H
