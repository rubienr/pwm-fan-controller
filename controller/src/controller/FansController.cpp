#include "FansController.h"
#include "fan/FansPwm.h"
#include "fan/FansTacho.h"
#include "temp/TempSensors.h"
#include <cmath>


FansController::FansController(TempSensors &sensors, FansPwm &pwms, FansTacho &tachos) :
sensors(sensors), pwms(pwms), tachos(tachos)
{
#if defined(FAN0)
    fansInfo[FAN0_INDEX].interpolator.setPowerCurvePoints(FAN0_CURVE_POWER, FAN0_CURVE_CENTI_CELSIUS);
    fansInfo[FAN0_INDEX].pwmSpecs = &pwms.getSpecs(FAN0_INDEX);
    fansInfo[FAN0_INDEX].rpmSpecs = &tachos.getSpecs(FAN0_INDEX);
    fansInfo[FAN0_INDEX].tempSpecs = &sensors.getSpecs(FAN0_TEMP_SENSOR_INDEX);
#endif
#if defined(FAN1)
    fansInfo[FAN1_INDEX].interpolator.setPowerCurvePoints(FAN1_CURVE_POWER, FAN1_CURVE_CENTI_CELSIUS);
    fansInfo[FAN1_INDEX].pwmSpecs = &pwms.getSpecs(FAN1_INDEX);
    fansInfo[FAN1_INDEX].rpmSpecs = &tachos.getSpecs(FAN1_INDEX);
    fansInfo[FAN1_INDEX].tempSpecs = &sensors.getSpecs(FAN1_TEMP_SENSOR_INDEX);
#endif
#if defined(FAN2)
    fansInfo[FAN2_INDEX].interpolator.setPowerCurvePoints(FAN2_CURVE_POWER, FAN2_CURVE_CENTI_CELSIUS);
    fansInfo[FAN2_INDEX].pwmSpecs = &pwms.getSpecs(FAN2_INDEX);
    fansInfo[FAN2_INDEX].rpmSpecs = &tachos.getSpecs(FAN2_INDEX);
    fansInfo[FAN2_INDEX].tempSpecs = &sensors.getSpecs(FAN2_TEMP_SENSOR_INDEX);
#endif
#if defined(FAN3)
    fansInfo[FAN3_INDEX].interpolator.setPowerCurvePoints(FAN3_CURVE_POWER, FAN3_CURVE_CENTI_CELSIUS);
    fansInfo[FAN3_INDEX].pwmSpecs = &pwms.getSpecs(FAN3_INDEX);
    fansInfo[FAN3_INDEX].rpmSpecs = &tachos.getSpecs(FAN3_INDEX);
    fansInfo[FAN3_INDEX].tempSpecs = &sensors.getSpecs(FAN3_TEMP_SENSOR_INDEX);
#endif
#if defined(FAN4)
    fansInfo[FAN4_INDEX].interpolator.setPowerCurvePoints(FAN4_CURVE_POWER, FAN4_CURVE_CENTI_CELSIUS);
    fansInfo[FAN4_INDEX].pwmSpecs = &pwms.getSpecs(FAN4_INDEX);
    fansInfo[FAN4_INDEX].rpmSpecs = &tachos.getSpecs(FAN4_INDEX);
    fansInfo[FAN4_INDEX].tempSpecs = &sensors.getSpecs(FAN4_TEMP_SENSOR_INDEX);
#endif
}


void FansController::process()
{
    sensors.requestTemperatureConversion();
    tachos.processEvery1000Ms();

#if defined(FAN0)
    updateFanInfo(FAN0_INDEX, FAN0_TEMP_SENSOR_INDEX);
#endif
#if defined(FAN1)
    updateFanInfo(FAN1_INDEX, FAN1_TEMP_SENSOR_INDEX);
#endif
#if defined(FAN2)
    updateFanInfo(FAN2_INDEX, FAN2_TEMP_SENSOR_INDEX);
#endif
#if defined(FAN3)
    updateFanInfo(FAN3_INDEX, FAN3_TEMP_SENSOR_INDEX);
#endif
#if defined(FAN4)
    updateFanInfo(FAN4_INDEX, FAN4_TEMP_SENSOR_INDEX);
#endif
}


void FansController::updateFanInfo(uint8_t fanIndex, uint8_t fanTempSensorIndex)
{
    sensors.fetchTemperatureCelsius(fanTempSensorIndex);

    FanInfo &info{ fansInfo[fanIndex] };
    if(info.rpmSpecs->hasError or info.tempSpecs->hasError) { pwms.setErrorPwm(fanIndex); }
    else
    {
        info.interpolator.interpolatePowerFromTemperature(info.tempSpecs->currentTempC);
        float powerRatio{ (static_cast<float>(info.interpolator.getInterpolatedPower()) / 255.0f) };
        info.interpolatedPowerPercent = powerRatio * 100.0f;
        auto newPowerPwm = static_cast<uint16_t>(roundf(powerRatio * static_cast<float>(info.pwmSpecs->maxDuty)));

        if(info.pwmSpecs->currentDuty == newPowerPwm) info.trend = 0;
        else if(info.pwmSpecs->currentDuty > newPowerPwm) info.trend = -1;
        else info.trend = 1;
        pwms.setPwm(fanIndex, newPowerPwm);
    }
}

const FanInfo &FansController::getFanInfo(uint8_t fanIndex) const { return fansInfo[fanIndex]; }
