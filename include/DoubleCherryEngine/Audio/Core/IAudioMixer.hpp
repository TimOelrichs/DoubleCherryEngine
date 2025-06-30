class IAudioMixer {
public:
    virtual void mix(float* outputBuffer, size_t frameCount, int sampleRate) = 0;
    virtual void setChannelVolume(int channel, float volume) = 0;
    virtual void setChannelEffect(int channel, std::shared_ptr<IAudioEffect> effect) = 0;
    virtual size_t getChannelCount() const = 0;
    virtual ~IAudioMixer() = default;
};
