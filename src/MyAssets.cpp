#include "MyAssets.h"

void MyAssets::init(Textures2DManager& textures) {
    sprites_pack.loadDir("data/sprites", GL_RGBA, 2048);
    Texture2D& sprites_tex = textures.addItem("sprites");
    sprites_tex.bind(tidSprites);
    sprites_tex.set(sprites_pack.getPackImage());
//    fonts_pack.addFont("data/fonts/arial.ttf", {{10,50}});
//    fonts_pack.pack();
//    Texture2D& fonts_tex = textures.addItem("fonts");
//    fonts_tex.bind(tidFonts);
//    fonts_tex.set(fonts_pack.getPackImage());
}