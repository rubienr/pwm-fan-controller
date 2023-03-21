#pragma once
#include "FanSpeedInterpolator.hpp"
#include "configuration.h"

struct TempSensors;
struct TempSensorSpecs;
struct FansPwm;
struct FanPwmSpecs;
struct FansTacho;
struct FanTachoSpecs;

struct FanInfo
{
    FanPwmSpecs *pwmSpecs{ nullptr };           // PWM related specs/values
    FanTachoSpecs *rpmSpecs{ nullptr };         // tacho related specs/counter
    TempSensorSpecs *tempSpecs{ nullptr };      // temperature related specs/values
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
    bool updateFanTempSensorIndex(uint8_t fanIndex, uint8_t tempSensorIndex);

protected:
    void updateFanInfo(uint8_t fanIndex, uint8_t fanTempSensorIndex);

    TempSensors &sensors;  // most likely for write
    FansPwm &pwms;         // most likely for write
    FansTacho &tachos;     // most likely for write
    FanInfo fansInfo[5]{}; // most likely for read only
};
