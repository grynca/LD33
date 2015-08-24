#include "geng.h"
#include "HUD.h"
#include "../MyGame.h"
#include "../MyAssets.h"
#include "../Layers.h"

HUD::HUD() {}


Entity& HUD::create(MyGame& game) {
//static
    Entity& ent = game.getModule<EntityManager>().addItem();
    ent.setRoles({erRenderable});
    HUD& me = ent.set<HUD>();
    me.createHUD(game);

    SpriteRenderable& sr = me.renderables.add<SpriteRenderable>(game.getModule<Window>());
    sr.init(layerHUD, 0, game.getModule<MyAssets>().sprites_pack.getRegions().at("data/sprites/gameover.png"));
    sr.setVisible(false);

    SpriteRenderable& sr2 = me.renderables.add<SpriteRenderable>(game.getModule<Window>());
    sr2.init(layerHUD, 0, game.getModule<MyAssets>().sprites_pack.getRegions().at("data/sprites/youwon.png"));
    sr2.setVisible(false);


    ViewPort& vp = game.getModule<Window>().getViewPort();
    SpriteRenderable& sr3 = me.renderables.add<SpriteRenderable>(game.getModule<Window>());
    sr3.init(layerHUD, 0, game.getModule<MyAssets>().sprites_pack.getRegions().at("data/sprites/health.png"));
    sr3.setFrame(fScreen);
    sr3.getLocalTransform().setPosition({110, vp.getBaseSize().getY()-25});

    SpriteRenderable& sr4 = me.renderables.add<SpriteRenderable>(game.getModule<Window>());
    sr4.init(layerHUD, 0, game.getModule<MyAssets>().sprites_pack.getRegions().at("data/sprites/laser.png"));
    sr4.setFrame(fScreen);
    sr4.getLocalTransform().setPosition({vp.getBaseSize().getX()-315, vp.getBaseSize().getY()-25});

    SpriteRenderable& sr5 = me.renderables.add<SpriteRenderable>(game.getModule<Window>());
    sr5.init(layerHUD, 0, game.getModule<MyAssets>().sprites_pack.getRegions().at("data/sprites/progress.png"));
    sr5.setFrame(fScreen);
    sr5.getLocalTransform().setPosition({115, 25});


    return ent;
}

void HUD::update(MyGame& game) {
    if (game.game_won)
        return;

    static float progress = 0;
    float h_w = game.getMonster().health/100*200;
    getHUD(hudHealthBar).setSize(Vec2(h_w, 30));

    float l_w = game.getMonster().laser/100*200;
    getHUD(hudLaserBar).setSize(Vec2(l_w, 30));

    ++progress;
    if (progress > end_progress) {
        game.game_won = true;
        game.helicopters.winGame();
        for (uint32_t i=0; i<game.missiles.getMissilesCount(); ++i) {
            game.missiles.getMissile(i).explode();
        }
        getYouWonLabel().setVisible(true);
    }
    float p_w = progress/end_progress * 800;
    getHUD(hudProgressBar).setSize(Vec2(p_w, 30));

}

void HUD::gameOver(MyGame& game) {
    getGameOverLabel().setVisible(true);
}

RectRenderable& HUD::getHUD(Hud h) {
    return *((RectRenderable*)renderables.get(h));
}

SpriteRenderable& HUD::getGameOverLabel() {
    return *((SpriteRenderable*)renderables.get(hudCount));
}

SpriteRenderable& HUD::getYouWonLabel() {
    return *((SpriteRenderable*)renderables.get(hudCount+1));
}

void HUD::createHUD(MyGame& game) {
    RectRenderable& healthbar = renderables.add<RectRenderable>(game.getModule<Window>(), layerHUD, Vec2(200, 30));
    healthbar.setColor(0, 1, 0, 1);
    healthbar.setFrame(fScreen);
    ViewPort& vp = game.getModule<Window>().getViewPort();
    healthbar.getLocalTransform().setPosition({315, vp.getBaseSize().getY()-30});

    RectRenderable& laserbar = renderables.add<RectRenderable>(game.getModule<Window>(), layerHUD, Vec2(200, 30));
    laserbar.setColor(1, 0, 0, 1);
    laserbar.setFrame(fScreen);
    laserbar.getLocalTransform().setPosition({vp.getBaseSize().getX()-115, vp.getBaseSize().getY()-30});

    RectRenderable& progressbar = renderables.add<RectRenderable>(game.getModule<Window>(), layerHUD, Vec2(0, 30));
    progressbar.setColor(1, 1, 1, 1);
    progressbar.setFrame(fScreen);
    progressbar.getLocalTransform().setPosition({215, 30});
}

