#include <libretro.h>
#include <DoubleCherryEngine/libretro/libretroVariables.h>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/CoreEventManager/CoreEventManager.hpp>
#include <DoubleCherryEngine/CoreEventManager/IEventListener.hpp>
#include <DoubleCherryEngine/Renderer/VideoRenderer/VideoFrameBufferManager.h>



class IVideoPostProcessor : public ISingleton<IVideoPostProcessor>, public IEventListener {
    friend class ISingleton<IVideoPostProcessor>;

public:

    virtual void applyPostProcessing() = 0;


protected:

    VideoFramebufferManager& videoFramebufferManager = VideoFramebufferManager::getInstance();
    uint8_t* currentFrameBuffer_ = nullptr;
    FramebufferInfo* currentFramebufferInfo_;

    IVideoPostProcessor() {
        CoreEventManager::getInstance().registerOnBeforeVideoRender(this);
    }

    void onBeforeVideoRender() override {
        currentFramebufferInfo_ = videoFramebufferManager.getCurrentFramebufferInfo();
        currentFrameBuffer_ = videoFramebufferManager.getCurrentFrameBuffer();
        doPostProsssing();
        videoFramebufferManager.setCurrentFrameBuffer(currentFrameBuffer_);
        videoFramebufferManager.setCurrentFramebufferInfo(currentFramebufferInfo_);
    }

};