#include "IAudioEffect.hpp"

class EffectChain : public IAudioEffect {
    std::vector<std::shared_ptr<IAudioEffect>> chain;

public:
    void add(std::shared_ptr<IAudioEffect> fx) {
        chain.push_back(std::move(fx));
    }

    void process(float* samples, size_t count, int sampleRate) override {
        for (auto& fx : chain) {
            fx->process(samples, count, sampleRate);
        }
    }
};
