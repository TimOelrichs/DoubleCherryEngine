#pragma once

#include <libretro.h>
#include <DoubleCherryEngine/libretro/libretroVariables.h>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/CoreEventManager/CoreEventManager.hpp>
#include <DoubleCherryEngine/CoreEventManager/IEventListener.hpp>

struct FramebufferInfo {
    int height;
    int width;
    int pitch;
	retro_pixel_format currentPixelFormat;
   
};


class VideoFramebufferManager final : public ISingleton<VideoFramebufferManager>, public IEventListener {
    friend class ISingleton<VideoFramebufferManager>;

public:

    uint8_t* getCurrentFrameBuffer() { return currentFrameBuffer_; };
    void setCurrentFrameBuffer(uint8_t* buffer) {
        currentFrameBuffer_ = buffer;
    };

    FramebufferInfo* getCurrentFramebufferInfo() { return currentFramebufferInfo_; };
    void setCurrentFramebufferInfo(FramebufferInfo* info) { currentFramebufferInfo_ = info; };

    void setPixelFormat(retro_pixel_format pixelFormat) { 
        currentFramebufferInfo_->currentPixelFormat = pixelFormat;
        environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &currentFramebufferInfo_->currentPixelFormat);
    };
    retro_pixel_format getPixelFormat() { return currentFramebufferInfo_->currentPixelFormat; };

private:
	uint8_t* currentFrameBuffer_ = nullptr;
    FramebufferInfo* currentFramebufferInfo_;

protected:

    bool onLoadGame() override {
        setPixelFormat(RETRO_PIXEL_FORMAT_RGB565);
    }

};

