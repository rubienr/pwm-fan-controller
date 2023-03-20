#pragma once
#include "FanSpeedInterpolator.h"
#include "configuration.h"
#include "fan/FansPwm.h"
#include "fan/FansTacho.h"
#include "temp/SensorsTemp.h"

struct FanInfo
{
    int16_t rpm{ 0 };
    uint8_t power{ 0 };
    uint16_t powerPwm{ 0 };
    float tempCelsius{ 0 };
    FanSpeedInterpolator interpolator{};
};


struct FansController
{
    FansController(SensorsTemp &sensors, FansPwm &pwms, FansTacho &tachos);
    void process();

    FanInfo fansInfo[5];

protected:
    void updateFanInfo(uint8_t fanIndex, uint8_t fanTempSensorIndex, uint16_t maxPwmDuty);

    SensorsTemp &sensors;
    FansPwm &pwms;
    FansTacho &tachos;
};
