#pragma once

#include <libretro.h>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/CoreEventManager/CoreEventManager.hpp>
#include <DoubleCherryEngine/CoreEventManager/IEventListener.hpp>
#include <DoubleCherryEngine/Renderer/VideoRenderer/VideoFrameBufferManager.h>


struct SystemScreenInfo {
    int height;
    int width;
    int pitch;
};


class VideoLayoutManager final : public ISingleton<VideoLayoutManager>, public IEventListener {
    friend class ISingleton<VideoLayoutManager>;

public:
    void getAVInfo(struct retro_system_av_info* info);
    void addVideoBuffer(int which, void* buffer);
    uint8_t* getCurrentFrameBuffer();
    SystemScreenInfo* getCurrentSystemScreenInfo();

private:

    void updateGeometry();

protected:

    bool onLoadGame() override {
        // Optionally: load core-specific options or preset here
    }
    void onOptionsUpdate() override {
        // Optionally: parse and apply new options here
    }

};
