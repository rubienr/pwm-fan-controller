#pragma once
#include "FanSpeedInterpolator.h"
#include "configuration.h"
#include "fan/FansPwm.h"
#include "fan/FansTacho.h"
#include "temp/SensorsTemp.h"

struct FanInfo
{
    int16_t rpm{ 0 };                    // measured RPM in last time period of 1000ms
    uint8_t power{ 0 };                  // 0 - 255
    float powerPercent{ 0.0f };          // power in %
    uint16_t powerPwm{ 0 };              // w.r.t. FANx_PWM_MAX_DUTY
    float tempCelsius{ 0 };              // last sensor conversion
    FanSpeedInterpolator interpolator{}; // interpolates according to configured power curve
    bool hasTachoError{ false };         // true if tacho readout was erroneous
    bool hasTempError{ false };          // true if temp sensor readout was erroneous
    bool hasPwmAlert{ false };           // true if PWM is not within FANx_ALERT_BELOW_PWM and FANx_ALERT_ABOVE_PWM
    bool hasRpmAlert{ false };           // true if RPM is not within FANx_ALERT_BELOW_RPM and FANx_ALERT_ABOVE_RPM
    bool hasTempAlert{ false };          // true if temperature is not within FANx_ALERT_BELOW_TEMP_C and FANx_ALERT_ABOVE_TEMP_C
    int8_t trend{ 0 };                   // -1, 0, 1 : slower, steady, faster
};


struct FansController
{
    FansController(SensorsTemp &sensors, FansPwm &pwms, FansTacho &tachos);
    void process();

    FanInfo fansInfo[5];

protected:
    void updateFanInfo(uint8_t fanIndex,
                       uint8_t fanTempSensorIndex,
                       uint16_t maxPwmDuty,
                       uint16_t fanErrorPwm,
                       uint16_t fanAlertBelowPwm,
                       uint16_t fanAlertAbovePwm,
                       uint16_t fanAlertBelowRpm,
                       uint16_t fanAlertAboveRpm,
                       float fanAlertBelowTempCelsius,
                       float fanAlertAboveTempCelsius);

    SensorsTemp &sensors;
    FansPwm &pwms;
    FansTacho &tachos;
    bool hasTachoError{ false };
};
