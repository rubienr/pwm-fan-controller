#include "FansController.h"
#include "fan/FansPwm.h"
#include "fan/FansTacho.h"
#include "fan/FansTemperature.h"
#include "sensors/TempSensors.h"
#include <cmath>


#define initFanInfo(n)                                                                                                                     \
    {                                                                                                                                      \
        fansInfo[FAN##n].interpolator.setPowerCurvePoints(FAN##n##_CURVE_POWER, FAN##n##_CURVE_DECI_CELSIUS);                              \
        fansInfo[FAN##n].pwmSpec = &fansPwm.getSpec(FAN##n);                                                                               \
        fansInfo[FAN##n].rpmSpec = &fansTacho.getSpec(FAN##n);                                                                             \
        fansInfo[FAN##n].fanTemperatureSpec = &fansTemperature.getSpec(FAN##n);                                                            \
        fansInfo[FAN##n].temperatureSensorSpec = &temperatureSensors.getSpec(fansInfo[FAN##n].fanTemperatureSpec->temperatureSensorIndex); \
    }


FansController::FansController(TempSensors &temperatureSensors, FansTemperature &fansTemperature, FansPwm &fansPwm, FansTacho &fansTacho) :
temperatureSensors(temperatureSensors), fansTemperature(fansTemperature), fansPwm(fansPwm), fansTacho(fansTacho)
{
    if(isFan0Defined) initFanInfo(0);
    if(isFan1Defined) initFanInfo(1);
    if(isFan2Defined) initFanInfo(2);
    if(isFan3Defined) initFanInfo(3);
    if(isFan4Defined) initFanInfo(4);
}


void FansController::process()
{
    temperatureSensors.requestTemperatureConversion();
    fansTacho.processEvery1000Ms();

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
    temperatureSensors.fetchTemperatureCelsius(fanTempSensorIndex);
    FanInfo &info{ fansInfo[fanIndex] };
    {
        const auto &temperatureSensorSpec{ temperatureSensors.getSpec(fanTempSensorIndex) };
        info.fanTemperatureSpec->currentTempC = temperatureSensorSpec.currentTempC;
    }

    if(info.rpmSpec->hasError or info.fanTemperatureSpec->hasError) { fansPwm.setErrorPwm(fanIndex); }
    else
    {
        info.interpolator.interpolatePowerFromTemperature(info.temperatureSensorSpec->currentTempC);
        float powerRatio{ (static_cast<float>(info.interpolator.getInterpolatedPower()) / 255.0f) };
        info.interpolatedPowerPercent = powerRatio * 100.0f;
        auto newPowerPwm = static_cast<uint16_t>(roundf(powerRatio * static_cast<float>(info.pwmSpec->maxDuty)));

        if(info.pwmSpec->currentDuty == newPowerPwm) info.trend = 0;
        else if(info.pwmSpec->currentDuty > newPowerPwm) info.trend = -1;
        else info.trend = 1;
        fansPwm.setPwm(fanIndex, newPowerPwm);
    }
}

const FanInfo &FansController::getFanInfo(uint8_t fanIndex) const { return fansInfo[fanIndex]; }


FanInfo &FansController::getFanInfo(uint8_t fanIndex) { return fansInfo[fanIndex]; }

bool FansController::updateFanTemperatureSensorIndex(uint8_t fanIndex, uint8_t tempSensorIndex)
{
    if(isFanDefined(fanIndex) && isTemperatureSensorDefined(tempSensorIndex))
    {
        fansInfo[fanIndex].fanTemperatureSpec->temperatureSensorIndex = tempSensorIndex;
        fansInfo[fanIndex].temperatureSensorSpec = &temperatureSensors.getSpec(tempSensorIndex);
        return true;
    }
    return false;
}


bool FansController::updateTemperatureSensorAddress(uint8_t tempSensorIndex, const uint8_t (&address)[8])
{
    return temperatureSensors.setDeviceAddress(tempSensorIndex, address);
}

bool FansController::getTemperatureSensorAddress(uint8_t tempSensorIndex, uint8_t (&address)[8])
{
    return temperatureSensors.getDeviceAddress(tempSensorIndex, address);
}
