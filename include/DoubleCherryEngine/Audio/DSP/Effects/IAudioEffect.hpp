class IAudioEffect {
public:
    virtual ~IAudioEffect() = default;
    virtual void process(float* samples, size_t count, int sampleRate) = 0;
};

