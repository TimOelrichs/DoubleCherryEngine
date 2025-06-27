#pragma once

#include <libretro.h>
#include <tuple>
#include <cmath>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/EngineCoreManager/EngineCoreManager.hpp>
#include <DoubleCherryEngine/EngineEventManager/EngineEventManager.hpp>
#include <DoubleCherryEngine/EngineEventManager/IEventListener.hpp>


class AudioRenderer final : public ISingleton<AudioRenderer>, public IEventListener {
    friend class ISingleton<AudioRenderer>;

public:
    void render();


private:






protected:

    bool onLoadGame() override {
  
    }

    void onOptionsUpdate() override {
  
    }

};
