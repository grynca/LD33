#ifndef ANIMATION_H
#define ANIMATION_H

#include "base.h"
#include "assets.h"
using namespace grynca;

class AnimationFrame {
public:
    AnimationFrame(const std::string& sprite_name, uint32_t time = 1);

    const std::string& getName()const;
    uint32_t getTime();
private:
    std::string sprite_name_;
    uint32_t time_;
};

class Animation {
public:
    void init(const ImagesPack& pack, const fast_vector<AnimationFrame>& frames,  bool loop);

    void toStart();
    void toEnd();
    void changeDir();

    // returns if frame changed
    bool update();

    const TextureRegion& getRegion()const;
    uint32_t getCurrectFrame()const;
    void setCurrentFrame(uint32_t frame_id);
    uint32_t getFramesCount()const;
    bool isAtEnd()const;
private:
    uint32_t current_frame_;
    uint32_t to_next_frame_;
    bool loop_;
    fast_vector<AnimationFrame> frames_;
    fast_vector<TextureRegion> texture_regions_;
};

#endif //ANIMATION_H
