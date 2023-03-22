#include "FansController.h"
#include "fan/FansPwm.h"
#include "fan/FansTacho.h"
#include "temp/TempSensors.h"
#include <cmath>


#define initFanInfo(n)                                                                                                 \
    {                                                                                                                  \
        fansInfo[FAN##n##_INDEX].interpolator.setPowerCurvePoints(FAN##n##_CURVE_POWER, FAN##n##_CURVE_CENTI_CELSIUS); \
        fansInfo[FAN##n##_INDEX].pwmSpecs = &pwms.getSpecs(FAN##n##_INDEX);                                            \
        fansInfo[FAN##n##_INDEX].rpmSpecs = &tachos.getSpecs(FAN##n##_INDEX);                                          \
        fansInfo[FAN##n##_INDEX].tempSpecs = &sensors.getSpecs(FAN##n##_TEMP_SENSOR_INDEX);                            \
    }


FansController::FansController(TempSensors &sensors, FansPwm &pwms, FansTacho &tachos) :
sensors(sensors), pwms(pwms), tachos(tachos)
{
    if(isFan0Defined) initFanInfo(0);
    if(isFan1Defined) initFanInfo(1);
    if(isFan2Defined) initFanInfo(2);
    if(isFan3Defined) initFanInfo(3);
    if(isFan4Defined) initFanInfo(4);
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


FanInfo &FansController::getFanInfo(uint8_t fanIndex) { return fansInfo[fanIndex]; }

bool FansController::updateFanTempSensorIndex(uint8_t fanIndex, uint8_t tempSensorIndex)
{
    if(isAnyFanWithIndexDefined(fanIndex) && tempSensorIndex < getDefinedTempSensorsCount())
    {
        fansInfo[fanIndex].tempSpecs = &sensors.getSpecs(tempSensorIndex);
        return true;
    }
    return false;
}
