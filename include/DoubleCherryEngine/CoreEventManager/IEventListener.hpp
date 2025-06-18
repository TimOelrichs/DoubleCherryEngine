#pragma once
#include <DoubleCherryEngine/CoreEventManager/CoreEventManager.hpp>

;
class IEventListener {
protected:
    virtual bool onLoadGame() {};
    virtual void onReset() {};
    virtual void onBeforeRun() {};
    virtual void onAfterRun() {};
    virtual void onBeforeVideoRender() {};
    virtual void onBeforeAudioRender() {};
    virtual void onOptionsUpdate() {};
    virtual void onSerialize() {};
    virtual void onUnserialize() {};
    virtual ~IEventListener() = default;

    friend class CoreEventManager;
};


