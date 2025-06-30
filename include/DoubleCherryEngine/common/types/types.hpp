#pragma once

enum SystemType {
    GB,
    GBA,
    NDS,
    SGB
};

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
    static constexpr ScreenSize SGB() { return { 256, 224, RETRO_PIXEL_FORMAT_RGB565 }; } 
};
