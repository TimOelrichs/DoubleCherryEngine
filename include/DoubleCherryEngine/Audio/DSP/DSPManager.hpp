class DSPManager {
    std::array<EffectChain, GB_CHANNEL_COUNT> channels;

public:
    void addEffect(int channel, std::unique_ptr<IAudioEffect> effect) {
        if (channel >= 0 && channel < GB_CHANNEL_COUNT)
            channels[channel].addEffect(std::move(effect));
    }

    Sample process(Sample input, int channel) {
        if (channel >= 0 && channel < GB_CHANNEL_COUNT)
            return channels[channel].process(input, channel);
        return input;
    }

    void processBuffer(Sample* inputBuffer, Sample* outputBuffer, int samplesPerChannel) {
        for (int ch = 0; ch < GB_CHANNEL_COUNT; ++ch) {
            for (int i = 0; i < samplesPerChannel; ++i) {
                int idx = ch * samplesPerChannel + i;
                outputBuffer[idx] = process(inputBuffer[idx], ch);
            }
        }
    }
};
