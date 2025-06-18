#pragma once
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/common/types/types.hpp>
#include <DoubleCherryEngine/CoreManager/ICore.hpp>
#include <DoubleCherryEngine/CoreEventManager/CoreEventManager.hpp>
#include <DoubleCherryEngine/CoreConfigurator/CoreConfigurator.hpp>


class CoreManager final : public ISingleton<CoreManager> {
    friend class ISingleton<CoreManager>;

public:

    void setCore(ICore* core) {
        core_ = core;
    }
  
    void deinit()  {
        unloadGame();
    }

    bool loadGame(const struct retro_game_info* info)  {
        core_ = CoreConfigurator::getInstance().getCore(info);
        CoreEventManager::getInstance().onLoadGame();
    }

    bool loadGameSpecial()  {
        return false;
    }

    void unloadGame()  {
        core_ = nullptr;
    }

    void reset()  {
        if (core_) core_->reset();
        CoreEventManager::getInstance().onReset();
    }

    void run()  {
        CoreEventManager::getInstance().onBeforeRun();
        if (core_) core_->run();
        CoreEventManager::getInstance().onAfterRun();
    }

private:
    ICore* core_ = nullptr;
protected:
    CoreManager() = default;
};
