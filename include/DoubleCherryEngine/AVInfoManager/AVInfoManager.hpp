#pragma once

#include <libretro.h>
#include <tuple>
#include <cmath>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/EngineCoreManager/EngineCoreManager.hpp>
#include <DoubleCherryEngine/EngineEventManager/EngineEventManager.hpp>
#include <DoubleCherryEngine/EngineEventManager/IEventListener.hpp>

enum class gridColumns {
    Horizontal,
    Vertical,
    Grid
};


struct AVSettings {
    float fps;
    unsigned sample_rate;
};

inline AVSettings getAVSettingsForSystem(SystemType sys) {
    switch (sys) {
    case SystemType::GB:  return { 59.73f, 44100 };
    case SystemType::GBA: return { 59.73f, 32768 };
    case SystemType::NDS: return { 59.73f, 32768 };
    default:             return { 60.0f, 44100 };
    }
}

class AVInfoManager final : public ISingleton<AVInfoManager>, public IEventListener {
    friend class ISingleton<AVInfoManager>;

public:

    void getAVInfo(struct retro_system_av_info* info) {
        if (!info) return;
        currentAVInfo = info;

        info->geometry = getGeometry();

        SystemType sysType = coreManager.getSystemType(); 
        AVSettings settings = getAVSettingsForSystem(sysType);

        info->timing.fps = settings.fps;
        info->timing.sample_rate = settings.sample_rate;

      
    }

    gridColumns getCurrentLayoutType() const {
        return currentLayoutType;
    }

    std::tuple<int, int> calculateGrid(int total, gridColumns layout) {
        switch (layout) {
        case gridColumns::Horizontal:
            return { total, 1 }; // cols = total, rows = 1
        case gridColumns::Vertical:
            return { 1, total }; // cols = 1, rows = total
        case gridColumns::Grid:
        default:
            int cols = static_cast<int>(std::ceil(std::sqrt(total)));
            int rows = static_cast<int>(std::ceil(static_cast<float>(total) / cols));
            return { cols, rows };
        }
    }

private:

    EngineCoreManager& coreManager = EngineCoreManager::getInstance();
    gridColumns currentLayoutType = gridColumns::Grid;
    int maxSystemsCount = 1;
    int currentActiveSystem = 1;
    struct retro_system_av_info* currentAVInfo = nullptr;
    ScreenSize currentScreenSize = ScreenSize::GB(); // Default
	SystemType currentSystemType = SystemType::GB; // Default

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
        updateGeometry();
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
