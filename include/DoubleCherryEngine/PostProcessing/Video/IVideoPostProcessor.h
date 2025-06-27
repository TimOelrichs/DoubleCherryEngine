#include <libretro.h>
#include <DoubleCherryEngine/libretro/libretroVariables.h>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/EngineEventManager/EngineEventManager.hpp>
#include <DoubleCherryEngine/EngineEventManager/IEventListener.hpp>




class IVideoPostProcessor : public ISingleton<IVideoPostProcessor>, public IEventListener {
    friend class ISingleton<IVideoPostProcessor>;

public:

    virtual void applyPostProcessing() = 0;


protected:

    IVideoPostProcessor() {
        EngineEventManager::getInstance().registerOnBeforeVideoRender(this);
    }

    void onBeforeVideoRender() override {
		//TODO: GET FRAME BUFFER FROM VIDEO POST PROCESSOR
        applyPostProcessing();
         //TODO: SEND FRAME BUFFER
    }

};