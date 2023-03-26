#pragma once
#include "FanSpeedInterpolator.hpp"
#include "configuration.h"

struct TempSensors;
struct TempSensorSpec;
struct FansPwm;
struct FanPwmSpec;
struct FansTacho;
struct FanTachoSpec;

struct FanInfo
{
    FanPwmSpec *pwmSpecs{ nullptr };            // PWM related specs/values
    FanTachoSpec *rpmSpecs{ nullptr };          // tacho related specs/counter
    TempSensorSpec *tempSpecs{ nullptr };       // temperature related specs/values
    float interpolatedPowerPercent{ 0.0f };     // power in %
    int8_t trend{ 0 };                          // -1, 0, 1 : slower, steady, faster
    FanSpeedInterpolator4Points interpolator{}; // interpolates according to configured power curve
};


struct FansController
{
    FansController(TempSensors &sensors, FansPwm &pwms, FansTacho &tachos);
    void process();
    [[nodiscard]] const FanInfo &getFanInfo(uint8_t fanIndex) const;
    [[nodiscard]] FanInfo &getFanInfo(uint8_t fanIndex);
    bool updateFanTemperatureSensorIndex(uint8_t fanIndex, uint8_t tempSensorIndex);
    bool updateTemperatureSensorAddress(uint8_t tempSensorIndex, const uint8_t (&address)[8]);

protected:
    void updateFanInfo(uint8_t fanIndex, uint8_t fanTempSensorIndex);

    TempSensors &sensors;  // most likely for write
    FansPwm &pwms;         // most likely for write
    FansTacho &tachos;     // most likely for write
    FanInfo fansInfo[5]{}; // most likely for read only
};
