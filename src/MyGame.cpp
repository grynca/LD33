#include "MyGame.h"
#include "MyAssets.h"
#include "entities/Monster.h"
#include "entities/HUD.h"
#include "entities/Missile.h"
#include <time.h>


MyGame::MyGame()
 : Game("LD33", 1024, 768) , game_won(false)
{

}

Monster& MyGame::getMonster() {
    return getModule<EntityManager>().getItem(monster_id_).get<Monster>();
}

HUD& MyGame::getHUD() {
    return getModule<EntityManager>().getItem(hud_id_).get<HUD>();
}

void MyGame::init() {
    srand(time(NULL));

    getModule<Window>().setClearingColor(100, 149, 237, 255);
#ifndef WEB
    getModule<Window>().setVSync(true);
#endif
    getModule<MyAssets>().init(getModule<Window>().getTextures());

    Monster::initResources(*this);
    Missile::initResources(*this);

    hud_id_= HUD::create(*this).getId();
    monster_id_ = Monster::create(*this).getId();
    helicopters.init(*this);
    missiles.init(*this);

//    MyEntity::initResources(*this);
//    Shuttle::initResources(*this);
//
//    me = &MyEntity::create(*this);
//    shuttle = &Shuttle::create(*this);
}

void MyGame::update() {
    getMonster().update(*this);
    helicopters.update();
    missiles.update();
    getHUD().update(*this);
//    me->get<MyEntity>().update();
//    shuttle->get<Shuttle>().update(*this);

#ifndef WEB
    updateDesktopShortcuts_();
#endif
    //updateCameraMotion_();


}

void MyGame::tick() {
    //std::cout << "FPS: " << getFPS() << std::endl;
    //std::cout << "UPS: " << getUPS() << std::endl;
}

void MyGame::updateDesktopShortcuts_() {
    Events& events = getModule<Window>().getEvents();
    bool alt_down = events.isKeyDown(SDL_SCANCODE_LALT)|events.isKeyDown(SDL_SCANCODE_RALT);
    if (events.wasKeyPressed(SDL_SCANCODE_F4) && alt_down)
        quit();
    else if (events.wasKeyPressed(SDL_SCANCODE_RETURN) && alt_down)
        getModule<Window>().toggleFullscreen();
}

void MyGame::updateCameraMotion_() {
    Events& events = getModule<Window>().getEvents();
    static float camera_l_speed = (float)100/60;
    Vec2 camera_motion;
    camera_motion.accX() -= (int)events.isKeyDown(SDL_SCANCODE_LEFT);
    camera_motion.accX() += (int)events.isKeyDown(SDL_SCANCODE_RIGHT);
    camera_motion.accY() -= (int)events.isKeyDown(SDL_SCANCODE_UP);
    camera_motion.accY() += (int)events.isKeyDown(SDL_SCANCODE_DOWN);
    if (camera_motion.getX() && camera_motion.getY()) {
        static float norm = 1.f/(float)sqrt(2);
        camera_motion *= norm;
    }
    camera_motion *= camera_l_speed;
    getModule<Window>().getViewPort().moveRelative(camera_motion);
}