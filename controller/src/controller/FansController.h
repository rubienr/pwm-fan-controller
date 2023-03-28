#pragma once
#include "FanSpeedInterpolator.hpp"
#include "configuration.h"

struct TempSensors;
struct TempSensorSpec;
struct FansPwm;
struct FanPwmSpec;
struct FansTacho;
struct FanTachoSpec;
struct FansTemperature;
struct FanTemperatureSpec;

struct FanInfo
{
    FanPwmSpec *pwmSpec{ nullptr };                    // PWM related specs/values
    FanTachoSpec *rpmSpec{ nullptr };                  // tacho related specs/counter
    FanTemperatureSpec *fanTemperatureSpec{ nullptr }; // fan temperature related specs/values
    TempSensorSpec *temperatureSensorSpec{ nullptr };  // temperature sensor related specs/values
    float interpolatedPowerPercent{ 0.0f };            // power in %
    int8_t trend{ 0 };                                 // -1, 0, 1 : slower, steady, faster
    FanSpeedInterpolator4Points interpolator{};        // interpolates according to configured power curve
};


struct FansController
{
    FansController(TempSensors &sensors, FansTemperature &fansTemperature, FansPwm &fansPwm, FansTacho &fansTacho);
    void process();
    [[nodiscard]] bool hasError(uint8_t fanIndex) const;
    [[nodiscard]] bool hasAlert(uint8_t fanIndex) const;
    [[nodiscard]] const FanInfo &getFanInfo(uint8_t fanIndex) const;
    [[nodiscard]] FanInfo &getFanInfo(uint8_t fanIndex);
    bool updateFanTemperatureSensorIndex(uint8_t fanIndex, uint8_t tempSensorIndex);
    bool updateTemperatureSensorAddress(uint8_t tempSensorIndex, const uint8_t (&address)[8]);
    bool getTemperatureSensorAddress(uint8_t tempSensorIndex, uint8_t (&address)[8]);

protected:
    void fetchTemperatureSensorData(const FanInfo &info) const;
    void updateFanInfo(uint8_t fanIndex);

    TempSensors &temperatureSensors;
    FansTemperature &fansTemperature;
    FansPwm &fansPwm;
    FansTacho &fansTacho;
    FanInfo fansInfo[5]{}; // bundled specs
};
