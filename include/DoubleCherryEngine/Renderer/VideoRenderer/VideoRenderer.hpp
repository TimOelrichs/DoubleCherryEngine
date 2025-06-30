#pragma once

#include <libretro.h>
#include <DoubleCherryEngine/libretro/libretroVariables.h>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/EngineCoreManager/EngineCoreManager.hpp>
#include <DoubleCherryEngine/EngineEventManager/EngineEventManager.hpp>
#include <DoubleCherryEngine/AVInfoManager/AVInfoManager.hpp>
#include <string>

class VideoRenderer final : public ISingleton<VideoRenderer>{
    friend class ISingleton<VideoRenderer>;

public:

    void init() {
        ScreenSize singleScreenSize = avInfoManager.getCurrentAvSettings().screenSize;
        screenW = singleScreenSize.width;
        screenH = singleScreenSize.height;
        screenPitch = singleScreenSize.getPitch();
        screensToRenderCount = coreManager.getActiveSystemsCount();
        layoutType = avInfoManager.getCurrentLayoutType();

        std::tie(gridColums, gridRows) = avInfoManager.calculateGrid(screensToRenderCount, layoutType);

        fullWidth = screenW * gridColums;
        fullHeight = screenH * gridRows;
        fullPitch = screenPitch * gridColums;

        frameBuffer.resize(fullPitch * fullHeight);
        std::fill(frameBuffer.begin(), frameBuffer.end(), 0);
        filledSlots = 0;
    };

    void addFrame(int index, const uint8_t* data) {

        if (index < 0 || index >= screensToRenderCount) return;
        int x = index % gridColums;
        int y = index / gridColums;

        for (int row = 0; row < screenH; ++row) {
            uint8_t* dst = frameBuffer.data() + fullPitch * (y * screenH + row) + x * screenPitch;
            const uint8_t* src = data + screenPitch * row;
            std::memcpy(dst, src, screenPitch);
        }

        if (++filledSlots == screensToRenderCount) render();
    };
 

    void displayMessage(std::string& msg_str, unsigned int seconds, bool alt)
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
    void displayMessage(std::string& msg_str) { displayMessage(msg_str, 5, false); };
    void displayMessage_alt(std::string& msg_str) { displayMessage(msg_str, 5, true); };



private:

    AVInfoManager& avInfoManager = AVInfoManager::getInstance();
    EngineEventManager& eventManager = EngineEventManager::getInstance();
	EngineCoreManager& coreManager = EngineCoreManager::getInstance();
    int screenW = 0, screenH = 0, screenPitch = 0;
    int screensToRenderCount = 0;
    int fullWidth = 0, fullHeight = 0, fullPitch = 0;
    int gridColums = 0, gridRows = 0;
    int filledSlots = 0;
    gridColumns layoutType = gridColumns::Grid;
    std::vector<uint8_t> frameBuffer;


    VideoRenderer() {
            init();
	};
 
    void render() {	
        filledSlots = 0;
        eventManager.onBeforeVideoRender();
        video_cb(frameBuffer.data(), fullWidth, fullHeight, fullPitch);	
    };

  
  
    
};
