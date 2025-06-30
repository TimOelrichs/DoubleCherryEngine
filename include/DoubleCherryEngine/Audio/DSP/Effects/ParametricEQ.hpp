#include "IAudioEffect.hpp"

class ParametricEQ : public IAudioEffect {
    float a0, a1, a2, b0, b1, b2;
    float x1 = 0.0f, x2 = 0.0f;
    float y1 = 0.0f, y2 = 0.0f;

public:
    ParametricEQ(float freq, float Q, float gainDB, int sampleRate) {
        float A = std::pow(10.0f, gainDB / 40.0f);
        float omega = 2.0f * M_PI * freq / sampleRate;
        float alpha = std::sin(omega) / (2.0f * Q);
        float cosw = std::cos(omega);

        b0 = 1 + alpha * A;
        b1 = -2 * cosw;
        b2 = 1 - alpha * A;
        a0 = 1 + alpha / A;
        a1 = -2 * cosw;
        a2 = 1 - alpha / A;

        // Normalize
        b0 /= a0; b1 /= a0; b2 /= a0;
        a1 /= a0; a2 /= a0;
    }

    void process(float* samples, size_t count, int sampleRate) override {
        for (size_t i = 0; i < count; ++i) {
            float x0 = samples[i];
            float y0 = b0 * x0 + b1 * x1 + b2 * x2
                - a1 * y1 - a2 * y2;

            x2 = x1;
            x1 = x0;
            y2 = y1;
            y1 = y0;

            samples[i] = y0;
        }
    }
};
