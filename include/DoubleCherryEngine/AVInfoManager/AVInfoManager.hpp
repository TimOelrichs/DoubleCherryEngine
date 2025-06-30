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
	ScreenSize screenSize;
};

inline AVSettings getAVSettingsForSystem(SystemType sys) {
    switch (sys) {
    case SystemType::GB:  return { 59.73f, 44100, ScreenSize::GB()};
    case SystemType::GBA: return { 59.73f, 32768, ScreenSize::GBA() };
    case SystemType::NDS: return { 59.73f, 32768, ScreenSize::NDS() };
    default:             return { 59.73f, 44100, ScreenSize::GB()};
    }
}

class AVInfoManager final : public ISingleton<AVInfoManager>, public IEventListener {
    friend class ISingleton<AVInfoManager>;

public:

    void updateAVInfo(struct retro_system_av_info* info) {
        if (!info) return;
        currentAVInfo = info;

        info->geometry = getGeometry();

        currentSystemType = coreManager.getSystemType();
        currentSettings = getAVSettingsForSystem(currentSystemType);

        info->timing.fps = currentSettings.fps;
        info->timing.sample_rate = currentSettings.sample_rate;
    }

    AVSettings getCurrentAvSettings() const {
        return currentSettings;
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

    SystemType currentSystemType = SystemType::GB; // Default system type
    AVSettings currentSettings = getAVSettingsForSystem(SystemType::GB);
	

    EngineCoreManager& coreManager = EngineCoreManager::getInstance();
    gridColumns currentLayoutType = gridColumns::Grid;
    int maxSystemsCount = 1;
    int currentActiveSystem = 1;
    struct retro_system_av_info* currentAVInfo = nullptr;
    ScreenSize currentScreenSize = ScreenSize::GB(); // Default


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
        currentScreenSize = getAVSettingsForSystem(currentSystemType).screenSize;
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
