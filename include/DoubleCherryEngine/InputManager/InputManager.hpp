#pragma once

#include <libretro.h>
#include <DoubleCherryEngine/libretro/libretroVariables.h>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/common/types/types.hpp>
#include <DoubleCherryEngine/EngineEventManager/IEventListener.hpp>
#include <array>
#include <unordered_map>

class InputManager final : public ISingleton<InputManager>, public IEventListener {
    friend class ISingleton<InputManager>;

public:
    retro_input_state_t getInputStateCallback() const {
        return input_state_cb;
    }

    void init() {
        auto& events = EngineEventManager::getInstance();
        events.registerOnOptionsUpdate(this);

        bool ok = environ_cb(RETRO_ENVIRONMENT_GET_SENSOR_INTERFACE, &sensorInterface_);
        if (!ok || !sensorInterface_)
            sensorInterface_ = nullptr;
    }

    const std::array<float, 3>& getGyroscope(unsigned port = 0) {
        activateGyroAccelSensors(port);
        for (int i = 0; i < 3; ++i)
            gyroValues_[i] = getSensorValue(port, RETRO_SENSOR_GYROSCOPE_X + i);
        return gyroValues_;
    }

    const std::array<float, 3>& getAccelerometer(unsigned port = 0) {
        activateGyroAccelSensors(port);
        for (int i = 0; i < 3; ++i)
            accelValues_[i] = getSensorValue(port, RETRO_SENSOR_ACCELEROMETER_X + i);
        return accelValues_;
    }

    float getIlluminance(unsigned port = 0) {
        activateLightSensor(port);
        return getSensorValue(port, RETRO_SENSOR_ILLUMINANCE);
    }

private:
    InputManager() = default;

    float getSensorValue(unsigned port, unsigned sensor_id) const {
        if (sensorInterface_ && sensorInterface_->get_sensor_input)
            return sensorInterface_->get_sensor_input(port, sensor_id);
        return 0.0f;
    }

    void activateGyroAccelSensors(unsigned port) {
        if (!gyroAccelEnabled_[port] && sensorInterface_ && sensorInterface_->set_sensor_state) {
            sensorInterface_->set_sensor_state(port, RETRO_SENSOR_ACCELEROMETER_ENABLE, 60);
            sensorInterface_->set_sensor_state(port, RETRO_SENSOR_GYROSCOPE_ENABLE, 60);
            gyroAccelEnabled_[port] = true;
        }
    }

    void activateLightSensor(unsigned port) {
        if (!lightSensorEnabled_[port] && sensorInterface_ && sensorInterface_->set_sensor_state) {
            sensorInterface_->set_sensor_state(port, RETRO_SENSOR_ILLUMINANCE_ENABLE, 10);
            lightSensorEnabled_[port] = true;
        }
    }

protected:
    void onOptionsUpdate() override {
        auto& options = CoreOptionsManager::getInstance();
        // Optional: Optionen auswerten
    }

private:
    const retro_sensor_interface* sensorInterface_ = nullptr;
    std::array<float, 3> accelValues_ = { 0.f, 0.f, 0.f };
    std::array<float, 3> gyroValues_ = { 0.f, 0.f, 0.f };

    // Flags pro Port for Sensoractivation
    std::unordered_map<unsigned, bool> gyroAccelEnabled_;
    std::unordered_map<unsigned, bool> lightSensorEnabled_;
};
