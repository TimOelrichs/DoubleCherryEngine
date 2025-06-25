#pragma once

#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/EngineEventManager/IEventListener.hpp>
#include <vector>
#include <algorithm>

class EngineEventManager : public ISingleton<EngineEventManager> {
    friend class ISingleton<EngineEventManager>;

public:
    // Notify all listeners to 
    void onLoadGame() {
        for (auto* listener : onLoadGameListeners)
            listener->onLoadGame();
    };

    // Notify all listeners to 
    void onReset() {
        for (auto* listener : onResetListeners)
            listener->onReset();
    };
  
    // Notify all listeners before the core run
    void onBeforeRun() {
        for (auto* listener : onBeforeRunListeners)
            listener->onBeforeRun();
    };

    // Notify all listeners after the core run
    void onAfterRun() {
        for (auto* listener : onAfterRunListeners)
            listener->onAfterRun();
    }

    // Notify all listeners before video rendering
    void onBeforeVideoRender() {
        for (auto* listener : onBeforeVideoRenderListeners)
            listener->onBeforeVideoRender();
    }

    // Notify all listeners before audio rendering
    void onBeforeAudioRender() {
        for (auto* listener : onBeforeAudioRenderListeners)
            listener->onBeforeAudioRender();
    }

    // Notify all listeners when options are updated
    void onOptionsUpdate() {
        for (auto* listener : onOptionsUpdateListeners)
            listener->onOptionsUpdate();
    }

    // Notify all listeners to serialize state
    void onSerialize() {
        for (auto* listener : onSerializeListeners)
            listener->onSerialize();
    }

    // Notify all listeners to unserialize state
    void onUnserialize() {
        for (auto* listener : onUnserializeListeners)
            listener->onUnserialize();
    }

    // Registration methods
    void registerOnLoadGame(IEventListener* listener) {
        registerListener(onBeforeRunListeners, listener);
    }

    void registerOnReset(IEventListener* listener) {
        registerListener(onResetListeners, listener);
    }

    void registerOnBeforeRun(IEventListener* listener) {
        registerListener(onBeforeRunListeners, listener);
    }

    void registerOnAfterRun(IEventListener* listener) {
        registerListener(onAfterRunListeners, listener);
    }

    void registerOnBeforeVideoRender(IEventListener* listener) {
        registerListener(onBeforeVideoRenderListeners, listener);
    }

    void registerOnBeforeAudioRender(IEventListener* listener) {
        registerListener(onBeforeAudioRenderListeners, listener);
    }

    void registerOnOptionsUpdate(IEventListener* listener) {
        registerListener(onOptionsUpdateListeners, listener);
    }

    void registerOnSerialize(IEventListener* listener) {
        registerListener(onSerializeListeners, listener);
    }

    void registerOnUnserialize(IEventListener* listener) {
        registerListener(onUnserializeListeners, listener);
    }

    // Unregistration methods
 
    void unregisterOnReset(IEventListener* listener) {
        unregisterListener(onResetListeners, listener);
    }
    void unregisterOnBeforeRun(IEventListener* listener) {
        unregisterListener(onBeforeRunListeners, listener);
    }

    void unregisterOnAfterRun(IEventListener* listener) {
        unregisterListener(onAfterRunListeners, listener);
    }

    void unregisterOnBeforeVideoRender(IEventListener* listener) {
        unregisterListener(onBeforeVideoRenderListeners, listener);
    }

    void unregisterOnBeforeAudioRender(IEventListener* listener) {
        unregisterListener(onBeforeAudioRenderListeners, listener);
    }

    void unregisterOnOptionsUpdate(IEventListener* listener) {
        unregisterListener(onOptionsUpdateListeners, listener);
    }

    void unregisterOnSerialize(IEventListener* listener) {
        unregisterListener(onSerializeListeners, listener);
    }

    void unregisterOnUnserialize(IEventListener* listener) {
        unregisterListener(onUnserializeListeners, listener);
    }

private:
    std::vector<IEventListener*> onLoadGameListeners;
    std::vector<IEventListener*> onResetListeners;
    std::vector<IEventListener*> onBeforeRunListeners;
    std::vector<IEventListener*> onAfterRunListeners;
    std::vector<IEventListener*> onBeforeVideoRenderListeners;
    std::vector<IEventListener*> onBeforeAudioRenderListeners;
    std::vector<IEventListener*> onOptionsUpdateListeners;
    std::vector<IEventListener*> onSerializeListeners;
    std::vector<IEventListener*> onUnserializeListeners;

    // Helper function to register listener if not already in the list
    void registerListener(std::vector<IEventListener*>& list, IEventListener* listener) {
        if (std::find(list.begin(), list.end(), listener) == list.end()) {
            list.push_back(listener);
        }
    }

    // Helper function to unregister a listener from the list
    void unregisterListener(std::vector<IEventListener*>& list, IEventListener* listener) {
        list.erase(std::remove(list.begin(), list.end(), listener), list.end());
    }
};
