#include <DoubleCherryEngine/PostProcessing/Video/IVideoPostProcessor.h>
#include <DoubleCherryEngine/CoreOptionsManager/ICoreOptionsManager.hpp>
#include <DoubleCherryEngine/Util/PixelUtil.h>

enum LcdInterlacingEffectType {
    LCD_INTERLACING_EFFECT_OFF = 0,
    LCD_INTERLACING_EFFECT_FAST = 1,
    LCD_INTERLACING_EFFECT_LINEAR = 2,
};

static const OptionEntry interlace_table[] = {
    { "Off",    LCD_INTERLACING_EFFECT_OFF },
    { "Fast",   LCD_INTERLACING_EFFECT_FAST },
    { "Linear", LCD_INTERLACING_EFFECT_LINEAR },
    { NULL,     0 }
};

class LcdInterlacingEffect : public IVideoPostProcessor {
public:
    void applyPostProcessing() override {
        applyLcdInterlacingEffect();
    }

private:
    CoreOptionsManager& optionsManager_ = CoreOptionsManager::getInstance();
    int is_odd_frame = 0;
    float brightenFactor = 1.05f; // default 5%
    LcdInterlacingEffectType lcdInterlacingEffectType = LCD_INTERLACING_EFFECT_FAST;

    void applyLcdInterlacingEffect() {
        if (lcdInterlacingEffectType == LCD_INTERLACING_EFFECT_OFF)
            return;

        int height = currentFramebufferInfo_->height;
        int width = currentFramebufferInfo_->width;
        int pitch = currentFramebufferInfo_->pitch;
        uint8_t* buf = currentFrameBuffer_;

        for (int y = is_odd_frame; y < height; y += 2) {
            uint16_t* line = reinterpret_cast<uint16_t*>(buf + y * pitch);

            if (lcdInterlacingEffectType == LCD_INTERLACING_EFFECT_FAST) {
                for (int x = 0; x < width; ++x) {
                    uint16_t pixel = line[x];
                    int r5 = PixelUtil::getRed(pixel);
                    int g6 = PixelUtil::getGreen(pixel);
                    int b5 = PixelUtil::getBlue(pixel);

                    r5 = std::min(int(r5 * brightenFactor), 31);
                    g6 = std::min(int(g6 * brightenFactor), 63);
                    b5 = std::min(int(b5 * brightenFactor), 31);

                    line[x] = PixelUtil::makeRGB565(r5, g6, b5);
                }
            }
            else {
                float hslStep = brightenFactor - 1.0f;
                for (int x = 0; x < width; ++x) {
                    line[x] = PixelUtil::brightenRgb565Hsl(line[x], hslStep);
                }
            }
        }

        is_odd_frame = (int)!(bool)is_odd_frame;
    }

    void onOptionsUpdate() override {
        lcdInterlacingEffectType = static_cast<LcdInterlacingEffectType>(
            optionsManager_.getEnumOption("dcgb_gbc_lcd_interlacing", interlace_table, LCD_INTERLACING_EFFECT_FAST));

        brightenFactor = static_cast<float>(
            optionsManager_.getIntOption("dcgb_gbc_lcd_interlacing_brightness", 5)) / 100.0f;
    }
};
