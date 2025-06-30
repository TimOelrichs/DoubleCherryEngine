class OctaverEffect : public IAudioEffect {
    float lastSample = 0.0f;
    bool even = false;

public:
    void process(float* samples, size_t count, int sampleRate) override {
        for (size_t i = 0; i < count; ++i) {
            if (even)
                samples[i] += lastSample * 0.5f;
            else
                lastSample = samples[i];
            even = !even;
        }
    }
};
