#ifndef HUD_H
#define HUD_H

using namespace grynca;
// fw
class MyGame;

class HUD {
public:
    HUD();

    //static void initResources(Game& game);
    static Entity& create(MyGame& game);

    void update(MyGame& game);

    void gameOver(MyGame& game);

    Transform transform;
    Renderables renderables;
private:
    enum Hud {
        hudHealthBar,
        hudLaserBar,
        hudProgressBar,

        hudCount
    };

    static constexpr float end_progress = 9000;

    void createHUD(MyGame& game);

    RectRenderable& getHUD(Hud h);
    SpriteRenderable& getGameOverLabel();
    SpriteRenderable& getYouWonLabel();
};

#endif //HUD_H
