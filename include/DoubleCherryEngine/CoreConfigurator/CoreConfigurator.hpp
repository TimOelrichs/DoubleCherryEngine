#pragma once
#include <libretro.h>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/EngineCoreManager/ICore.hpp>
#include <DoubleCherryEngine/libretro/libretroVariables.h>


class CoreConfigurator : public ISingleton<CoreConfigurator> {
    friend class ISingleton<CoreConfigurator>;

public:
    void init() {};
   
    IMultiCore* getCore(const struct retro_game_info* info) {
        return nullptr;
    };

  

};