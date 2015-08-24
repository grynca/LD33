#ifndef MYASSETS_H
#define MYASSETS_H

#include "geng.h"
using namespace grynca;

class MyAssets {
public:
    enum TextureIds {
        tidSprites = 0,
        tidFonts = 1
    };

    void init(Textures2DManager& textures);

    ImagesPack sprites_pack;
    FontsPack fonts_pack;
};

#endif //MYASSETS_H
