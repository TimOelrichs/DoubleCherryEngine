#pragma once
#include <DoubleCherryEngine/common/types/types.hpp>
#include <string.h>



struct ScreenSize {
    int width;
    int height;
    retro_pixel_format format;

    constexpr int getPixelSizeBytes() const {
        switch (format) {
        case RETRO_PIXEL_FORMAT_0RGB1555: return 2;
        case RETRO_PIXEL_FORMAT_XRGB8888: return 4;
        case RETRO_PIXEL_FORMAT_RGB565:   return 2;
        }
        return 0; // fallback
    }

    constexpr int getPitch() const {
        return width * getPixelSizeBytes();
    }

    constexpr int getBufferSizeBytes() const {
        return getPitch() * height;
    }

    // Presets
    static constexpr ScreenSize GB() { return { 160, 144, RETRO_PIXEL_FORMAT_RGB565 }; }
    static constexpr ScreenSize GBA() { return { 240, 160, RETRO_PIXEL_FORMAT_RGB565 }; }
    static constexpr ScreenSize NDS() { return { 256, 192, RETRO_PIXEL_FORMAT_RGB565 }; }
};


class IMultiCore {
public:
    virtual ~IMultiCore() = default;


    SystemType getSystemType() const {
        return systemType;
	}
    virtual int getMaxSystemsCount() = 0;
    virtual int getActiveSystemsCount() = 0;


    // Initialize the core
    virtual void init() = 0;

    // Deinitialize and clean up the core
    virtual void deinit() = 0;

    // Load a standard game
    virtual bool loadGame(const struct retro_game_info* info) = 0;

    // Load a game with special parameters or behavior
    virtual bool loadGameSpecial() = 0;

    // Unload the currently loaded game
    virtual void unloadGame() = 0;

    // Reset the core state (e.g., like a soft reset)
    virtual void reset() = 0;

    // Run the core's main loop (e.g., for one frame)
    virtual void run() = 0;

    //SystemType systemType;
    //std::String gameTitle;
private:
	SystemType systemType = SystemType::GB; // Default to GB
};
