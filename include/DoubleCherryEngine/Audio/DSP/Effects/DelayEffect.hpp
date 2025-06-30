#include "IAudioEffect.hpp"

class DelayEffect : public IAudioEffect {
    std::vector<float> buffer;
    size_t delaySamples;
    float feedback;
    size_t index = 0;

public:
    DelayEffect(size_t delayMs, float feedbackAmount, int sampleRate)
        : delaySamples((delayMs* sampleRate) / 1000), feedback(feedbackAmount)
    {
        buffer.resize(delaySamples, 0.0f);
    }

    void process(float* samples, size_t count, int sampleRate) override {
        for (size_t i = 0; i < count; ++i) {
            float delayed = buffer[index];
            buffer[index] = samples[i] + delayed * feedback;
            samples[i] += delayed;
            index = (index + 1) % delaySamples;
        }
    }
};
