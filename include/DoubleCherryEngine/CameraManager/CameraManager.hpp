#pragma once

#include <libretro.h>
#include <DoubleCherryEngine/libretro/libretroVariables.h>
#include <cstring>

class CameraManager {
public:
    static CameraManager& getInstance() {
        static CameraManager instance;
        return instance;
    }

    void initialize(retro_environment_t retroEnv) {
        environmentCallback_ = retroEnv;

        if (!cameraSupported()) return;

        retro_camera_callback cb = {};
        cb.start = &CameraManager::cameraStartStatic;
        cb.stop = &CameraManager::cameraStopStatic;
        cb.frame_raw_framebuffer = &CameraManager::cameraFrameStatic;
        cb.userdata = this;

        environmentCallback_(RETRO_ENVIRONMENT_SET_CAMERA_CALLBACK, &cb);
    }

    bool cameraSupported() const {
        bool supported = false;
        if (environmentCallback_)
            environmentCallback_(RETRO_ENVIRONMENT_GET_CAMERA_INTERFACE_SUPPORT, &supported);
        return supported;
    }

    const uint8_t* getLastFrame() const {
        return lastFrame_;
    }

    uint32_t getFrameWidth() const {
        return frameWidth_;
    }

    uint32_t getFrameHeight() const {
        return frameHeight_;
    }

    bool isFrameAvailable() const {
        return frameAvailable_;
    }

    void consumeFrame() {
        frameAvailable_ = false;
    }

private:
    retro_environment_t environmentCallback_ = nullptr;

    static constexpr uint32_t MAX_FRAME_SIZE = 640 * 480 * 2;
    uint8_t lastFrame_[MAX_FRAME_SIZE] = {};
    uint32_t frameWidth_ = 0;
    uint32_t frameHeight_ = 0;
    bool frameAvailable_ = false;

    CameraManager() = default;
    ~CameraManager() = default;
    CameraManager(const CameraManager&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;

    static bool cameraStartStatic(void* userdata, uint32_t width, uint32_t height) {
        auto* self = static_cast<CameraManager*>(userdata);
        self->frameWidth_ = width;
        self->frameHeight_ = height;
        return true;
    }

    static void cameraStopStatic(void* userdata) {
        auto* self = static_cast<CameraManager*>(userdata);
        self->frameWidth_ = 0;
        self->frameHeight_ = 0;
    }

    static void cameraFrameStatic(void* userdata, const uint8_t* data, uint32_t width, uint32_t height, size_t pitch) {
        auto* self = static_cast<CameraManager*>(userdata);

        const size_t copyWidth = std::min<size_t>(width * 2, sizeof(self->lastFrame_) / height);
        const size_t maxHeight = std::min<size_t>(height, sizeof(self->lastFrame_) / copyWidth);

        for (size_t y = 0; y < maxHeight; ++y) {
            memcpy(&self->lastFrame_[y * copyWidth], &data[y * pitch], copyWidth);
        }

        self->frameAvailable_ = true;
        self->frameWidth_ = width;
        self->frameHeight_ = height;
    }
};
