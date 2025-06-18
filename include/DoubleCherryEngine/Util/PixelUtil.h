#pragma once
#include <cstdint>
#include <tuple>
#include <algorithm>

class PixelUtil {
public:

    // Extract 5-bit red component from RGB565
    static inline int getRed(uint16_t pixel) {
        return (pixel >> 11) & 0x1F;
    }

    // Extract 6-bit green component from RGB565
    static inline int getGreen(uint16_t pixel) {
        return (pixel >> 5) & 0x3F;
    }

    // Extract 5-bit blue component from RGB565
    static inline int getBlue(uint16_t pixel) {
        return pixel & 0x1F;
    }

    // Optional: convert to 8-bit range (0–255)
    static inline int getRed8(uint16_t pixel) {
        int r5 = getRed(pixel);
        return (r5 << 3) | (r5 >> 2); // 5-bit to 8-bit
    }

    static inline int getGreen8(uint16_t pixel) {
        int g6 = getGreen(pixel);
        return (g6 << 2) | (g6 >> 4); // 6-bit to 8-bit
    }

    static inline int getBlue8(uint16_t pixel) {
        int b5 = getBlue(pixel);
        return (b5 << 3) | (b5 >> 2); // 5-bit to 8-bit
    }

	//Convert RGB565 pixel to 4-bit grayscale (for GB Camera)
    static uint8_t rgbToGray4(uint16_t pixel) {
       
        int r5 = (pixel >> 11) & 0x1F;
        int g6 = (pixel >> 5) & 0x3F;
        int b5 = pixel & 0x1F;

    
        int r = (r5 * 255) / 31;
        int g = (g6 * 255) / 63;
        int b = (b5 * 255) / 31;

        int brightness = (r * 299 + g * 587 + b * 114) / 1000;

        if (brightness < 64) return 0;
        if (brightness < 128) return 1;
        if (brightness < 192) return 2;
        return 3;
    }

    //4-bit grayscale to 2bpp (for GB Camera)
    void packTo2bpp(const uint8_t* gray4, uint8_t* packed, int count) {
        for (int i = 0; i < count / 4; ++i) {
            uint8_t a = gray4[i * 4 + 0] & 0x03;
            uint8_t b = gray4[i * 4 + 1] & 0x03;
            uint8_t c = gray4[i * 4 + 2] & 0x03;
            uint8_t d = gray4[i * 4 + 3] & 0x03;

            packed[i] = (a << 6) | (b << 4) | (c << 2) | d;
        }
    }




// Set RGB565 pixel from 5-bit red, 6-bit green, and 5-bit blue
static inline uint16_t makeRGB565(int r5, int g6, int b5) {
    return (static_cast<uint16_t>(r5 & 0x1F) << 11) |
        (static_cast<uint16_t>(g6 & 0x3F) << 5) |
        (static_cast<uint16_t>(b5 & 0x1F));
}

    // Clamp float to 0.0 - 1.0
    static inline float clampf(float x, float minVal = 0.0f, float maxVal = 1.0f) {
        return std::min(std::max(x, minVal), maxVal);
    }

    // Convert hue to RGB component
    static inline float hueToRgb(float p, float q, float t) {
        if (t < 0.0f) t += 1.0f;
        if (t > 1.0f) t -= 1.0f;
        if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
        if (t < 1.0f / 2.0f) return q;
        if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
        return p;
    }

    // Convert RGB (8-bit) to HSL
    static inline std::tuple<float, float, float> rgbToHsl(int r, int g, int b) {
        float rf = r / 255.0f;
        float gf = g / 255.0f;
        float bf = b / 255.0f;

        float maxVal = std::max({ rf, gf, bf });
        float minVal = std::min({ rf, gf, bf });
        float h, s, l = (maxVal + minVal) * 0.5f;

        if (maxVal == minVal) {
            h = s = 0.0f;
        }
        else {
            float d = maxVal - minVal;
            s = (l > 0.5f) ? d / (2.0f - maxVal - minVal) : d / (maxVal + minVal);

            if (maxVal == rf)
                h = (gf - bf) / d + (gf < bf ? 6.0f : 0.0f);
            else if (maxVal == gf)
                h = (bf - rf) / d + 2.0f;
            else
                h = (rf - gf) / d + 4.0f;

            h /= 6.0f;
        }

        return { h, s, l };
    }

    // Convert HSL to RGB (8-bit)
    static inline std::tuple<int, int, int> hslToRgb(float h, float s, float l) {
        float rf, gf, bf;

        if (s == 0.0f) {
            rf = gf = bf = l;
        }
        else {
            float q = (l < 0.5f) ? l * (1.0f + s) : l + s - l * s;
            float p = 2.0f * l - q;
            rf = hueToRgb(p, q, h + 1.0f / 3.0f);
            gf = hueToRgb(p, q, h);
            bf = hueToRgb(p, q, h - 1.0f / 3.0f);
        }

        return {
            static_cast<int>(clampf(rf) * 255.0f),
            static_cast<int>(clampf(gf) * 255.0f),
            static_cast<int>(clampf(bf) * 255.0f)
        };
    }

    // Brighten a RGB565 pixel using HSL lightness adjustment
    static inline uint16_t brightenRgb565Hsl(uint16_t pixel, float brightenStep) {
        int r5 = (pixel >> 11) & 0x1F;
        int g6 = (pixel >> 5) & 0x3F;
        int b5 = pixel & 0x1F;

        // Convert to 8-bit RGB
        int r8 = (r5 << 3) | (r5 >> 2);
        int g8 = (g6 << 2) | (g6 >> 4);
        int b8 = (b5 << 3) | (b5 >> 2);

        auto [h, s, l] = rgbToHsl(r8, g8, b8);
        l = clampf(l + brightenStep);

        auto [newR, newG, newB] = hslToRgb(h, s, l);

        // Convert back to RGB565
        uint16_t newR5 = newR >> 3;
        uint16_t newG6 = newG >> 2;
        uint16_t newB5 = newB >> 3;

        return (newR5 << 11) | (newG6 << 5) | newB5;
    }

    // Convert RGB888 to RGB565
    static inline uint16_t rgb888ToRgb565(uint8_t r, uint8_t g, uint8_t b) {
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }
};
