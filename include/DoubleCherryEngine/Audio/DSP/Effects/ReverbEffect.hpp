#include "IAudioEffect.hpp"

class ReverbEffect : public IAudioEffect {
    std::vector<float> buffer;
    size_t delaySamples;
    float decay;
    size_t index = 0;

public:
    ReverbEffect(size_t delayMs, float decayFactor, int sampleRate)
        : delaySamples((delayMs* sampleRate) / 1000), decay(decayFactor)
    {
        buffer.resize(delaySamples, 0.0f);
    }

    void process(float* samples, size_t count, int sampleRate) override {
        for (size_t i = 0; i < count; ++i) {
            float delayed = buffer[index];
            buffer[index] = samples[i] + delayed * decay;
            samples[i] += delayed * decay;
            index = (index + 1) % delaySamples;
        }
    }
};
