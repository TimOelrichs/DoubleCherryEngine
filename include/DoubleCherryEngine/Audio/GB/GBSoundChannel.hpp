#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <memory>

class GbSoundChannel {
public:
    enum class Type {
        Square1,
        Square2,
        Wave,
        Noise
    };

    GbSoundChannel(Type type, const std::string& name)
        : m_type(type), m_name(name), m_volume(1.0f), m_enabled(true) {
    }

    // Add one audio sample (mono, 16-bit signed)
    void addSample(int16_t sample) {
        m_samples.push_back(sample);
    }

    // Get all current samples
    const std::vector<int16_t>& getSamples() const {
        return m_samples;
    }

    // Clear all collected samples (usually called after processing)
    void clearSamples() {
        m_samples.clear();
    }

    // Channel metadata
    Type getType() const {
        return m_type;
    }

    const std::string& getName() const {
        return m_name;
    }

    // Volume control
    void setVolume(float volume) {
        m_volume = volume;
    }

    float getVolume() const {
        return m_volume;
    }

    // Enable/disable channel
    void setEnabled(bool enabled) {
        m_enabled = enabled;
    }

    bool isEnabled() const {
        return m_enabled;
    }

    // Apply volume to all samples (simple gain stage)
    void applyVolume() {
        for (auto& sample : m_samples) {
            sample = static_cast<int16_t>(sample * m_volume);
        }
    }

private:
    Type m_type;
    std::string m_name;

    float m_volume;
    bool m_enabled;

    std::vector<int16_t> m_samples;
};