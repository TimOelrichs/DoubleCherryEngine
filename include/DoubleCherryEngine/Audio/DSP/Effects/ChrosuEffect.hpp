#include "IAudioEffect.hpp"

class ChorusEffect : public IAudioEffect {
    std::vector<float> buffer;
    float rateHz;
    float depthMs;
    size_t sampleRate;
    size_t writeIndex = 0;
    float phase = 0.0f;

public:
    ChorusEffect(float rateHz, float depthMs, int sampleRate)
        : rateHz(rateHz), depthMs(depthMs), sampleRate(sampleRate)
    {
        size_t maxDelaySamples = static_cast<size_t>((depthMs + 1) * sampleRate / 1000);
        buffer.resize(maxDelaySamples, 0.0f);
    }

    void process(float* samples, size_t count, int sampleRate_) override {
        for (size_t i = 0; i < count; ++i) {
            buffer[writeIndex] = samples[i];

            float lfo = (std::sin(phase * 2.0f * M_PI) + 1.0f) * 0.5f;
            size_t delaySamples = static_cast<size_t>(depthMs * lfo * sampleRate / 1000);
            size_t readIndex = (writeIndex + buffer.size() - delaySamples) % buffer.size();

            samples[i] += buffer[readIndex] * 0.5f;

            writeIndex = (writeIndex + 1) % buffer.size();
            phase += rateHz / sampleRate;
            if (phase > 1.0f) phase -= 1.0f;
        }
    }
};

