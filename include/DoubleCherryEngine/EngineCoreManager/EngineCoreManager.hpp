#pragma once
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/common/types/types.hpp>
#include <DoubleCherryEngine/EngineCoreManager/ICore.hpp>
#include <DoubleCherryEngine/EngineEventManager/EngineEventManager.hpp>
#include <DoubleCherryEngine/CoreConfigurator/CoreConfigurator.hpp>


class EngineCoreManager final : public ISingleton<EngineCoreManager> {
    friend class ISingleton<EngineCoreManager>;

public:

    void setCore(IMultiCore* core) {
        core_ = core;
    }

    void deinit()  {
        unloadGame();
    }

    bool loadGame(const struct retro_game_info* info)  {
        core_ = coreConfigurator_.getCore(info);
        eventManager_.onLoadGame();
		return core_ != nullptr;
    }

    bool loadGameSpecial()  {
        return false;
    }

    void unloadGame()  {
        core_ = nullptr;
    }

    void reset()  {
        if (core_) core_->reset();
        eventManager_.onReset();
    }

    void run()  {
        eventManager_.onBeforeRun();
        if (core_) core_->run();
        eventManager_.onAfterRun();
    }

    int getMaxSystemsCount() const {
        if (core_) return core_->getMaxSystemsCount();
        return 0;
    }

    int getActiveSystemsCount() const {
        if (core_) return core_->getActiveSystemsCount();
        return 0;
    }

    ScreenSize getSingleScreenSize() const {
        if (core_) return core_->getSingleScreenSize();
        return { 0, 0 };
    };

    int getSystemsFPS() const {
        if (core_) return core_->getSystemsFPS();
        return 0;
    };

    int getSystemsSampleRate() const {
        if (core_) return core_->getSystemsSampleRate();
        return 0;
    };

private:
	EngineEventManager& eventManager_ = EngineEventManager::getInstance();
	CoreConfigurator& coreConfigurator_ = CoreConfigurator::getInstance();
    IMultiCore* core_ = nullptr;
protected:
    EngineCoreManager() = default;
};
