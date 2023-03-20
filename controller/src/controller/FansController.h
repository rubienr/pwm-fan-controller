#pragma once
#include "FanSpeedInterpolator.h"
#include "configuration.h"

struct TempSensors;
struct TempSensorSpecs;
struct FansPwm;
struct FanPwmSpecs;
struct FansTacho;
struct FanTachoSpecs;

struct FanInfo
{
    const FanPwmSpecs *pwmSpecs{ nullptr };      // PWM related specs/values
    const FanTachoSpecs *rpmSpecs{ nullptr };    // tacho related specs/counter
    const TempSensorSpecs *tempSpecs{ nullptr }; // temperature related specs/values
    float interpolatedPowerPercent{ 0.0f };      // power in %
    int8_t trend{ 0 };                           // -1, 0, 1 : slower, steady, faster
    FanSpeedInterpolator interpolator{};         // interpolates according to configured power curve
};


struct FansController
{
    FansController(TempSensors &sensors, FansPwm &pwms, FansTacho &tachos);
    void process();
    [[nodiscard]] const FanInfo &getFanInfo(uint8_t fanIndex) const;

protected:
    void updateFanInfo(uint8_t fanIndex, uint8_t fanTempSensorIndex);

    TempSensors &sensors;  // write only
    FansPwm &pwms;         // write only
    FansTacho &tachos;     // write only
    FanInfo fansInfo[5]{}; // read only
};
