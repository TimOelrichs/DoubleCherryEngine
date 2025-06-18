#pragma once
#include <libretro.h>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/CoreManager/ICore.hpp>
#include <DoubleCherryEngine/libretro/libretroVariables.h>


class CoreConfigurator : public ISingleton<CoreConfigurator> {
    friend class ISingleton<CoreConfigurator>;

public:
    void init() {};
   
    ICore* getCore(const struct retro_game_info* info) {


    };

    retro_system_av_info* getAvInfo(struct retro_system_av_info* info) {
        avInfo_ = info;
    };

private:
    struct retro_system_av_info* avInfo_;

    void updateGeometry();
};