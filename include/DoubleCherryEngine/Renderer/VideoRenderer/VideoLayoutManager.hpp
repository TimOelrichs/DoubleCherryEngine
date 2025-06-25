#pragma once

#include <libretro.h>
#include <tuple>
#include <cmath>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/EngineCoreManager/EngineCoreManager.hpp>
#include <DoubleCherryEngine/EngineEventManager/EngineEventManager.hpp>
#include <DoubleCherryEngine/EngineEventManager/IEventListener.hpp>




enum class LayoutType {
    Horizontal,
    Vertical,
    Grid
};

class VideoLayoutManager final : public ISingleton<VideoLayoutManager>, public IEventListener {
    friend class ISingleton<VideoLayoutManager>;

public:

    void getAVInfo(struct retro_system_av_info* info) {
        if (!info) return;
        info->geometry = getGeometry();
        info->timing.fps = coreManager.getSystemsFPS();
        info->timing.sample_rate = coreManager.getSystemsSampleRate();
        currentAVInfo = info;
    }

    LayoutType getCurrentLayoutType() const {
        return currentLayoutType;
    }

    std::tuple<int, int> calculateGrid(int total, LayoutType layout) {
        switch (layout) {
        case LayoutType::Horizontal:
            return { total, 1 }; // cols = total, rows = 1
        case LayoutType::Vertical:
            return { 1, total }; // cols = 1, rows = total
        case LayoutType::Grid:
        default:
            int cols = static_cast<int>(std::ceil(std::sqrt(total)));
            int rows = static_cast<int>(std::ceil(static_cast<float>(total) / cols));
            return { cols, rows };
        }
    }



private:

    EngineCoreManager& coreManager = EngineCoreManager::getInstance();
    LayoutType currentLayoutType = LayoutType::Grid;
    int maxSystemsCount = 0;
    int currentActiveSystem = 0;
    struct retro_system_av_info* currentAVInfo = nullptr;
    ScreenSize currentScreenSize;
   

    retro_game_geometry getGeometry() {
        retro_game_geometry geometry;
        int width = currentScreenSize.width;
        int height = currentScreenSize.height;

        geometry.max_width = width * maxSystemsCount;
        geometry.max_height = height * maxSystemsCount;

        auto [cols, rows] = calculateGrid(currentActiveSystem, currentLayoutType);
        geometry.base_width = width * cols;
        geometry.base_height = height * rows;
        geometry.aspect_ratio = static_cast<float>(geometry.base_width) / geometry.base_height;

        return geometry;
    }

    void updateGeometry() {
        if (!currentAVInfo) return;
        currentAVInfo->geometry = getGeometry();
        environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, currentAVInfo);
    }



protected:

    bool onLoadGame() override {
        maxSystemsCount = coreManager.getMaxSystemsCount();
        currentActiveSystem = coreManager.getActiveSystemsCount();
        currentScreenSize = coreManager.getSingleScreenSize();
        return true;
    }

    void onOptionsUpdate() override {
        int activeNow = coreManager.getActiveSystemsCount();
        if (activeNow != currentActiveSystem) {
            currentActiveSystem = activeNow;
            updateGeometry();
        }
    }

};
