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
    updateFanInfo(FAN0);
#endif
#if defined(FAN1)
    updateFanInfo(FAN1);
#endif
#if defined(FAN2)
    updateFanInfo(FAN2);
#endif
#if defined(FAN3)
    updateFanInfo(FAN3);
#endif
#if defined(FAN4)
    updateFanInfo(FAN4);
#endif
}


bool FansController::hasError(uint8_t fanIndex) const
{
    return fansTemperature.getSpec(fanIndex).hasError || fansPwm.getSpec(fanIndex).hasError || fansTacho.getSpec(fanIndex).hasError;
}


bool FansController::hasAlert(uint8_t fanIndex) const
{
    return fansTemperature.getSpec(fanIndex).hasAlert() || fansPwm.getSpec(fanIndex).hasAlert() || fansTacho.getSpec(fanIndex).hasAlert();
}


void FansController::fetchTemperatureSensorData(const FanInfo &info) const
{
    temperatureSensors.fetchTemperatureCelsius(info.fanTemperatureSpec->temperatureSensorIndex);
    const auto &temperatureSensorSpec{ temperatureSensors.getSpec(fansInfo->fanTemperatureSpec->temperatureSensorIndex) };
    info.fanTemperatureSpec->currentTempC = temperatureSensorSpec.currentTempC;
    info.fanTemperatureSpec->hasError = temperatureSensorSpec.hasError;
}


void FansController::updateFanInfo(uint8_t fanIndex)
{
    FanInfo &info{ fansInfo[fanIndex] };
    fetchTemperatureSensorData(info);

    if(hasError(fanIndex) || forcedErrorState)
    {
        fansPwm.setErrorPwm(fanIndex);
        info.interpolatedPowerPercent = 100.0f * static_cast<float>(info.pwmSpec->errorDuty) / static_cast<float>(info.pwmSpec->maxDuty);
        info.trend = 0;
    }
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


void FansController::setForcedErrorState(bool forcedErrorFlag) { forcedErrorState = forcedErrorFlag; }
