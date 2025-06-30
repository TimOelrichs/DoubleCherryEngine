// File: Audio/GB/GBSoundMixer.hpp
#pragma once

#include "Audio/Core/IAudioMixer.hpp"
#include "Audio/DSP/DSPManager.hpp"
#include "GBSoundChannel.hpp"

class GBSoundMixer : public IAudioMixer {
public:
    GBSoundMixer();

    void mix(float* outputBuffer, size_t frameCount, int sampleRate) override;
    void setChannelVolume(int channel, float volume) override;
    void setChannelEffect(int channel, std::shared_ptr<IAudioEffect> effect) override;
    size_t getChannelCount() const override;

private:
    static constexpr size_t channelCount = 4; // GB: 4 channels
    std::array<GBSoundChannel, channelCount> channels;
    DSPManager dspManager;
};