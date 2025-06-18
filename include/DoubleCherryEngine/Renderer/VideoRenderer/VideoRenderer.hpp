#pragma once

#include <libretro.h>
#include <DoubleCherryEngine/libretro/libretroVariables.h>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/CoreEventManager/CoreEventManager.hpp>
#include <DoubleCherryEngine/Renderer/VideoRenderer/VideoLayoutManager.hpp>
#include <DoubleCherryEngine/Renderer/VideoRenderer/VideoFrameBufferManager.h>
#include <string>

class VideoRender final : public ISingleton<VideoRender>{
    friend class ISingleton<VideoRender>;

public:

    void render() {
        CoreEventManager::getInstance().onBeforeVideoRender();
        FramebufferInfo* screen = videoFramebufferManager.getCurrentFramebufferInfo();
		uint8_t* frame_buffer = videoFramebufferManager.getCurrentFrameBuffer();
        video_cb(frame_buffer,screen->width, screen->height, screen->pitch);
    };

    void displayMessage(std::string msg_str, unsigned int seconds, bool alt)
    {
        seconds %= 10;

        if (libretro_msg_interface_version >= 1)
        {
            struct retro_message_ext msg = {
               msg_str.data(),
               seconds * 1000,
               1,
               RETRO_LOG_INFO,
               RETRO_MESSAGE_TARGET_OSD,
               alt ? RETRO_MESSAGE_TYPE_NOTIFICATION_ALT : RETRO_MESSAGE_TYPE_NOTIFICATION,
               -1
            };
            environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE_EXT, &msg);
        }
        else
        {
            struct retro_message msg = {
                msg_str.data(),
               seconds * 60
            };
            environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &msg);
        }
    };
    void displayMessage(std::string msg_str) { displayMessage(msg_str, 5, false); };
    void displayMessage_alt(std::string msg_str) { displayMessage(msg_str, 5, true); };

private:

	VideoFramebufferManager& videoFramebufferManager = VideoFramebufferManager::getInstance();

    VideoRender() {
    };
    VideoLayoutManager& videoLayoutManager = VideoLayoutManager::getInstance();

    
};
