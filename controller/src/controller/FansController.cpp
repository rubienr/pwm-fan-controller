#include "FansController.h"
#include "fan/FansPwm.h"
#include "fan/FansTacho.h"
#include "temp/TempSensors.h"
#include <cmath>


#define initFanInfo(n)                                                                                        \
    {                                                                                                         \
        fansInfo[FAN##n].interpolator.setPowerCurvePoints(FAN##n##_CURVE_POWER, FAN##n##_CURVE_DECI_CELSIUS); \
        fansInfo[FAN##n].pwmSpecs = &pwms.getSpecs(FAN##n);                                                   \
        fansInfo[FAN##n].rpmSpecs = &tachos.getSpecs(FAN##n);                                                 \
        fansInfo[FAN##n].tempSpecs = &sensors.getSpecs(FAN##n##_TEMP_SENSOR_INDEX);                           \
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
    updateFanInfo(FAN0, FAN0_TEMP_SENSOR_INDEX);
#endif
#if defined(FAN1)
    updateFanInfo(FAN1, FAN1_TEMP_SENSOR_INDEX);
#endif
#if defined(FAN2)
    updateFanInfo(FAN2, FAN2_TEMP_SENSOR_INDEX);
#endif
#if defined(FAN3)
    updateFanInfo(FAN3, FAN3_TEMP_SENSOR_INDEX);
#endif
#if defined(FAN4)
    updateFanInfo(FAN4, FAN4_TEMP_SENSOR_INDEX);
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

bool FansController::updateFanTemperatureSensorIndex(uint8_t fanIndex, uint8_t tempSensorIndex)
{
    if(isFanDefined(fanIndex) && isTemperatureSensorDefined(tempSensorIndex))
    {
        fansInfo[fanIndex].tempSpecs = &sensors.getSpecs(tempSensorIndex);
        return true;
    }
    return false;
}


bool FansController::updateTemperatureSensorAddress(uint8_t tempSensorIndex, const uint8_t (&address)[8])
{
    return sensors.setDeviceAddress(tempSensorIndex, address);
}
