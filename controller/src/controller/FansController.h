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
    FansController(SensorsTemp &sensors, FansPwm &pwms, FansTacho &tachos) : sensors(sensors), pwms(pwms), tachos(tachos)
    {
#if defined(FAN0)
        fansInfo[FAN0_INDEX].interpolator.setCurvePoints(FAN0_CURVE_POWER, FAN0_CURVE_CENTI_CELSIUS);
#endif
#if defined(FAN1)
        fansInfo[FAN1_INDEX].interpolator.setCurvePoints(FAN1_CURVE_POWER, FAN1_CURVE_CENTI_CELSIUS);
#endif
#if defined(FAN2)
        fansInfo[FAN2_INDEX].interpolator.setCurvePoints(FAN2_CURVE_POWER, FAN2_CURVE_CENTI_CELSIUS);
#endif
#if defined(FAN3)
        fansInfo[FAN3_INDEX].interpolator.setCurvePoints(FAN3_CURVE_POWER, FAN3_CURVE_CENTI_CELSIUS);
#endif
#if defined(FAN4)
        fansInfo[FAN4_INDEX].interpolator.setCurvePoints(FAN4_CURVE_POWER, FAN4_CURVE_CENTI_CELSIUS);
#endif
    }


    void process()
    {
        sensors.captureTemperature(); // todo - ret error
        tachos.processEverySecond();  // todo - ret error
#if defined(FAN0)
        updateFan(FAN0_INDEX, FAN0_TEMP_SENSOR_INDEX, FAN0_PWM_MAX_DUTY);
#endif
#if defined(FAN1)
        updateFan(FAN1_INDEX, FAN1_TEMP_SENSOR_INDEX, FAN1_PWM_MAX_DUTY);
#endif
#if defined(FAN2)
        updateFan(FAN2_INDEX, FAN2_TEMP_SENSOR_INDEX, FAN2_PWM_MAX_DUTY);
#endif
#if defined(FAN3)
        updateFan(FAN3_INDEX, FAN3_TEMP_SENSOR_INDEX, FAN3_PWM_MAX_DUTY);
#endif
#if defined(FAN4)
        updateFan(FAN4_INDEX, FAN4_TEMP_SENSOR_INDEX, FAN4_PWM_MAX_DUTY);
#endif
    }

    FanInfo fansInfo[5];

protected:
    void updateFan(uint8_t fanIndex, uint8_t fanTempSensorIndex, uint16_t maxPwmDuty)
    {
        fansInfo[fanIndex].rpm = tachos.getRpm(fanIndex);
        sensors.readTemperatureCelsius(fanTempSensorIndex, fansInfo[fanIndex].tempCelsius);
        fansInfo[fanIndex].power = fansInfo[fanIndex].interpolator.interpolate(fansInfo[fanIndex].tempCelsius);
        fansInfo[fanIndex].powerPwm = fansInfo[fanIndex].power; // (255 / fansInfo[fanIndex].power) * maxPwmDuty;
        pwms.setPwm(fanIndex, fansInfo[fanIndex].powerPwm);
    }
    SensorsTemp &sensors;
    FansPwm &pwms;
    FansTacho &tachos;
};
