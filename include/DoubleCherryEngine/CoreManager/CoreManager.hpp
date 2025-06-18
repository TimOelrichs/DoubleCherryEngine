#pragma once
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/common/types/types.hpp>
#include <DoubleCherryEngine/CoreManager/ICore.hpp>
#include <DoubleCherryEngine/CoreEventManager/CoreEventManager.hpp>
#include <DoubleCherryEngine/CoreConfigurator/CoreConfigurator.hpp>


class CoreManager final : public ISingleton<CoreManager>, public ICore {
    friend class ISingleton<CoreManager>;

public:

    void setCore(ICore* core) {
        core_ = core;
    }
  
    void init() override {
        // Optionally prepare things before the core is selected
    }

    void deinit() override {
        unloadGame();
    }

    bool loadGame(const struct retro_game_info* info) override {
        core_ = CoreConfigurator::getInstance().getCore(info);
        CoreEventManager::getInstance().onLoadGame();
    }

    bool loadGameSpecial() override {
        return false;
    }

    void unloadGame() override {
        core_ = nullptr;
    }

    void reset() override {
        if (core_) core_->reset();
        CoreEventManager::getInstance().onReset();
    }

    void run() override {
        CoreEventManager::getInstance().onBeforeRun();
        if (core_) core_->run();
        CoreEventManager::getInstance().onAfterRun();
    }

private:
    ICore* core_ = nullptr;
protected:
    CoreManager() = default;
};
