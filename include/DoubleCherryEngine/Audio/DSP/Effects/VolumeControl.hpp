#include "IAudioEffect.hpp"

class VolumeControl : public IAudioEffect {
    float volume;
public:
    VolumeControl(float initialVolume = 1.0f) : volume(initialVolume) {}
    void setVolume(float v) { volume = std::clamp(v, 0.0f, 1.0f); }
    Sample process(Sample input, int) override { return input * volume; }
};