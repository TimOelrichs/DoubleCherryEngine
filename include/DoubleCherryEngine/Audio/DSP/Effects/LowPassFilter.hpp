#include "IAudioEffect.hpp"

class LowPassFilter : public IAudioEffect {
    float cutoff = 800.0f;
    float prev = 0.0f;
public:
    explicit LowPassFilter(float cutoffHz) : cutoff(cutoffHz) {}
    void process(float* samples, size_t count, int sampleRate) override {
        float alpha = cutoff / (cutoff + sampleRate);
        for (size_t i = 0; i < count; ++i) {
            samples[i] = prev + alpha * (samples[i] - prev);
            prev = samples[i];
        }
    }