#include "Animation.h"
#include <algorithm>

AnimationFrame::AnimationFrame(const std::string& sprite_name, uint32_t time)
 : sprite_name_(sprite_name), time_(time)
{}

const std::string& AnimationFrame::getName()const {
    return sprite_name_;
}

uint32_t AnimationFrame::getTime() {
    return time_;
}

void Animation::init(const ImagesPack& pack, const fast_vector<AnimationFrame>& frames,  bool loop) {
    ASSERT(frames.size(), "Animation must contain at least 1 frame.");
    loop_=loop;
    frames_ = frames;
    for (uint32_t i=0; i<frames.size(); ++i) {
        texture_regions_.push_back(pack.getRegions().at(frames[i].getName()));
    }
    toStart();
}

void Animation::toStart() {
    setCurrentFrame(0);
}

void Animation::toEnd() {
    setCurrentFrame(uint32_t(frames_.size()-1));
}

void Animation::changeDir() {
    std::reverse(frames_.begin(), frames_.end());
    std::reverse(texture_regions_.begin(), texture_regions_.end());
    setCurrentFrame(uint32_t(frames_.size()-current_frame_-1));
}

bool Animation::update() {
    if (to_next_frame_ == 0)
        return false;
    --to_next_frame_;
    if (to_next_frame_ == 0) {
        uint32_t next_frame = current_frame_+1;
        if (next_frame == frames_.size()) {
            if (loop_)
                next_frame = 0;
            else
                return false;
        }
        setCurrentFrame(next_frame);
        return true;
    }
    return true;
}

const TextureRegion& Animation::getRegion()const {
    return texture_regions_[current_frame_];
}

uint32_t Animation::getCurrectFrame()const {
    return current_frame_;
}

void Animation::setCurrentFrame(uint32_t frame_id) {
    current_frame_ = frame_id;
    to_next_frame_ = frames_[current_frame_].getTime();
}

uint32_t Animation::getFramesCount()const {
    return frames_.size();
}

bool Animation::isAtEnd()const {
    return current_frame_ == frames_.size()-1;
}

