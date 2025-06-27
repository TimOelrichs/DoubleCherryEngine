#pragma once

#include <libretro.h>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <cstring>
#include <algorithm>

class CameraManager final : public ISingleton<CameraManager> {
    friend class ISingleton<CameraManager>;

public:
    void initialize(retro_environment_t env_cb) {
        environmentCallback_ = env_cb;

        retro_camera_callback cb{};
        cb.caps = (1ULL << RETRO_CAMERA_BUFFER_RAW_FRAMEBUFFER);
        cb.width = 640;
        cb.height = 480;
        cb.start = &CameraManager::cameraStartStatic;
        cb.stop = &CameraManager::cameraStopStatic;
        cb.frame_raw_framebuffer = &CameraManager::cameraFrameStatic;
        cb.frame_opengl_texture = nullptr;
        cb.initialized = nullptr;
        cb.deinitialized = nullptr;

        // Nur registrieren, wenn das Interface verfügbar ist
        retro_camera_callback* dummy = nullptr;
        if (env_cb && env_cb(RETRO_ENVIRONMENT_GET_CAMERA_INTERFACE, &dummy)) {
            env_cb(RETRO_ENVIRONMENT_SET_CAMERA_CALLBACK, &cb);
        }
    }

    const uint8_t* getLastFrame() const { return lastFrame_; }
    uint32_t getFrameWidth() const { return frameWidth_; }
    uint32_t getFrameHeight() const { return frameHeight_; }
    bool isFrameAvailable() const { return frameAvailable_; }
    void consumeFrame() { frameAvailable_ = false; }

private:
    CameraManager() = default;
    ~CameraManager() = default;
    CameraManager(const CameraManager&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;

    static bool cameraStartStatic(unsigned width, unsigned height) {
        auto& self = getInstance();
        self.frameWidth_ = width;
        self.frameHeight_ = height;
        self.frameAvailable_ = false;
        return true;
    }

    static void cameraStopStatic() {
        auto& self = getInstance();
        self.frameWidth_ = 0;
        self.frameHeight_ = 0;
        self.frameAvailable_ = false;
    }

    static void cameraFrameStatic(const uint8_t* data, unsigned width, unsigned height, size_t pitch) {
        auto& self = getInstance();

        constexpr size_t MAX_FRAME_SIZE = sizeof(self.lastFrame_);
        const size_t bytesPerPixel = 2;
        const size_t copyWidth = std::min(width * bytesPerPixel, pitch);
        const size_t maxHeight = std::min(height, MAX_FRAME_SIZE / copyWidth);

        for (size_t y = 0; y < maxHeight; ++y) {
            std::memcpy(&self.lastFrame_[y * copyWidth], &data[y * pitch], copyWidth);
        }

        self.frameWidth_ = width;
        self.frameHeight_ = height;
        self.frameAvailable_ = true;
    }

    retro_environment_t environmentCallback_ = nullptr;
    static constexpr size_t MAX_FRAME_SIZE = 640 * 480 * 2;
    uint8_t lastFrame_[MAX_FRAME_SIZE]{};
    uint32_t frameWidth_ = 0;
    uint32_t frameHeight_ = 0;
    bool frameAvailable_ = false;
};
