#include "IAudioEffect.hpp"

class HighPassFilter : public IAudioEffect {
    float cutoff;
    float a0, a1, b1;
    float prevInput = 0.0f;
    float prevOutput = 0.0f;
    bool initialized = false;

public:
    HighPassFilter(float cutoffHz, int sampleRate) : cutoff(cutoffHz) {
        float c = std::tan(M_PI * cutoffHz / sampleRate);
        float a = 1.0f / (1.0f + c);
        a0 = a;
        a1 = -a;
        b1 = (1.0f - c) * a;
        initialized = true;
    }

    void process(float* samples, size_t count, int sampleRate) override {
        if (!initialized) return;
        for (size_t i = 0; i < count; ++i) {
            float input = samples[i];
            float output = a0 * input + a1 * prevInput + b1 * prevOutput;
            prevInput = input;
            prevOutput = output;
            samples[i] = output;
        }
    }
};
