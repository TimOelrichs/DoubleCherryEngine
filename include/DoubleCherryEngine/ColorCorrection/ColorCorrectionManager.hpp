#pragma once

#include <libretro.h>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/EngineCoreManager/EngineCoreManager.hpp>
#include <DoubleCherryEngine/EngineEventManager/EngineEventManager.hpp>
#include <DoubleCherryEngine/EngineEventManager/IEventListener.hpp>


class ColorCorrectionManager final : public ISingleton<ColorCorrectionManager>, public IEventListener {
    friend class ISingleton<ColorCorrectionManager>;

public:
    mapColor()

};