#pragma once

#include <libretro.h>
#include <DoubleCherryEngine/libretro/libretroVariables.h>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/common/types/types.hpp>
#include <DoubleCherryEngine/EngineEventManager/IEventListener.hpp>
#include <array>


class InputManager final : public ISingleton<InputManager>, public IEventListener {
    friend class ISingleton<InputManager>;

public:
    retro_input_state_t getInputState() {
        return input_state_cb;
    }

    void init() {
        auto& events = EngineEventManager::getInstance();
        events.registerOnOptionsUpdate(this);
    };

    const std::array<float, 3>& getGyroscope() {

        /* TODO FIX
        for (int axis = 0; axis < 3; ++axis) {
                gyroValues_[axis] = input_state_cb(0, RETRO_DEVICE_SENSOR_GYROSCOPE, 0, axis);
            }
        */
        return gyroValues_;
    }

    const std::array<float, 3>& getAccelerometer() {

        /* TODO FIX
            for (int axis = 0; axis < 3; ++axis) {
                accelValues_[axis] = input_state_cb(0, RETRO_DEVICE_SENSOR_ACCELEROMETER, 0, axis);
            }
            */
        return accelValues_;
    }



private:

    // libretro Sensor Interface
    const retro_sensor_interface* sensorInterface_ = nullptr;

    InputManager() {

        bool ok = environ_cb(RETRO_ENVIRONMENT_GET_SENSOR_INTERFACE, &sensorInterface_);
        if (!ok || !sensorInterface_) {
            sensorInterface_ = nullptr; 
        }

      
    }

protected:
    void onOptionsUpdate() override {
        auto& options = CoreOptionsManager::getInstance();

    }

private:
    std::array<float, 3> accelValues_ = { 0.0f, 0.0f, 0.0f };
    std::array<float, 3> gyroValues_ = { 0.0f, 0.0f, 0.0f };

};
