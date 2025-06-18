#include <DoubleCherryEngine/PostProcessing/Video/IVideoPostProcessor.h>
#include <DoubleCherryEngine/CoreOptionsManager/ICoreOptionsManager.hpp>

enum LcdInterlacingEffectType {
    LCD_INTERLACING_EFFECT_OFF = 0,
    LCD_INTERLACING_EFFECT_FAST = 1,
    LCD_INTERLACING_EFFECT_LINEAR = 2,
};

//table for optionsManager_.getEnumOption
static const OptionEntry interlace_table[] = {
 { "Off",    LCD_INTERLACING_EFFECT_OFF },
 { "Fast",   LCD_INTERLACING_EFFECT_FAST },
 { "Linear", LCD_INTERLACING_EFFECT_LINEAR },
 { NULL,     0 }
};

class LcdInterlacingEffect : public IVideoPostProcessor {

public:

    void applyPostProsssing() override {
        applyLcdInterlacingEffect();
    };

private:
    ICoreOptionsManager& optionsManager_ = ICoreOptionsManager::getInstance();
    int isOddFrame = 0;
    float brightnessFactor = 1.05f;
    LcdInterlacingEffectType lcdInterlacingEffectType = LCD_INTERLACING_EFFECT_FAST;

    void applyLcdInterlacingEffect() {

        if (lcdInterlacingEffectType == LCD_INTERLACING_EFFECT_OFF) return;

        int height = this->currentFramebufferInfo_->height;
        int width = this->currentFramebufferInfo_->width;
        int pitch = this->currentFramebufferInfo_->pitch;
        uint8_t* buf = currentFrameBuffer_;

        for (int y = isOddFrame; y < height; y += 2) {

            uint16_t* line = reinterpret_cast<uint16_t*>(buf + y * pitch);


            if (lcdInterlacingEffectType == LCD_INTERLACING_EFFECT_FAST)
            {
                for (int x = 0; x < width; ++x) {
                    uint16_t pixel = line[x];

                    int r = (pixel >> 11) & 0x1F;
                    int g = (pixel >> 5) & 0x3F;
                    int b = pixel & 0x1F;

                    r = std::min(int(r * brightnessFactor), 31);
                    g = std::min(int(g * brightnessFactor), 63);
                    b = std::min(int(b * brightnessFactor), 31);

                    line[x] = (r << 11) | (g << 5) | b;
                }
            }
            else {

                for (int x = 0; x < width; ++x) {
                    line[x] = brighten_rgb565_hsl(line[x], (brightnessFactor - 1.0f));
                }
            }

        }
        isOddFrame = (int)!(bool)isOddFrame;
    };

    void onOptionsUpdate() override {

        lcdInterlacingEffectType = static_cast<LcdInterlacingEffectType>(optionsManager_.getEnumOption("dcgb_gbc_lcd_interlacing", interlace_table, LCD_INTERLACING_EFFECT_FAST));
        lcd_interlacing_brightness = (100.0f + (float)optionsManager_.getIntOption("dcgb_gbc_lcd_interlacing_brightnesss") / 100.0f;
    };
};


