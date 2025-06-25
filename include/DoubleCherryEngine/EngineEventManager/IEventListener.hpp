#pragma once
#include <DoubleCherryEngine/EngineEventManager/EngineEventManager.hpp>

;
class IEventListener {
protected:
    virtual bool onLoadGame() { return true; };
    virtual void onReset() {};
    virtual void onBeforeRun() {};
    virtual void onAfterRun() {};
    virtual void onBeforeVideoRender() {};
    virtual void onBeforeAudioRender() {};
    virtual void onOptionsUpdate() {};
    virtual void onSerialize() {};
    virtual void onUnserialize() {};
    virtual ~IEventListener() = default;

    friend class EngineEventManager;
};


