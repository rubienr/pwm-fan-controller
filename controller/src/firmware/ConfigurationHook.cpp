#include "ConfigurationHook.h"
#include "../fan/FansPwm.h"
#include "../fan/FansTacho.h"
#include "../fan/FansTemperature.h"
#include "../sensors/TempSensors.h"


StoreStatus ConfigurationHook::storeRamToFlash()
{
    restoreSettingsFromRam();
    return settings.storeSettings();
}


LoadStatus ConfigurationHook::restoreRamFromFlash()
{
    const auto loadStatus{ settings.loadSettings() };
    if(LoadStatus::Loaded == loadStatus) restoreRamFromSettings();
    return loadStatus;
}


bool ConfigurationHook::resetRamSettings()
{
    settings.getSettings().reset();
    restoreRamFromSettings();
    return true;
}


void ConfigurationHook::reportSettings() const { settings.reportContainer(); }


ConfigurationHook::ConfigurationHook(FansController &controller, FlashSettings &settings, unsigned long &autoreportDelayS) :
controller(controller), settings(settings), autoreportDelayS(autoreportDelayS)
{
}


bool ConfigurationHook::setAutoreportDelaySeconds(const unsigned long &autoreportDelay)
{
    autoreportDelayS = autoreportDelay;
    return true;
}


const unsigned long &ConfigurationHook::getAutoreportDelaySeconds() { return autoreportDelayS; }


bool ConfigurationHook::setPwmAlert(uint8_t fanIndex, const uint32_t &minPwm, const uint32_t &maxPwm)
{
    FanPwmSpec &pwmSpec{ *controller.getFanInfo(fanIndex).pwmSpec };
    pwmSpec.alertBelowDuty = minPwm;
    pwmSpec.alertAboveDuty = maxPwm;
    return true;
}


bool ConfigurationHook::setTemperatureAlert(uint8_t fanIndex, const int16_t &minTempDeciC, const int16_t &maxTempDeciC)
{
    FanTemperatureSpec &fanTempSpec{ *controller.getFanInfo(fanIndex).fanTemperatureSpec };
    const float tempMin{ static_cast<float>(minTempDeciC) * 10.0f }, tempMax{ static_cast<float>(maxTempDeciC) * 10.0f };
    fanTempSpec.alertBelowTempC = tempMin;
    fanTempSpec.alertAboveTempC = tempMax;
    return true;
}


bool ConfigurationHook::setRpmAlert(uint8_t fanIndex, const uint32_t &minRpm, const uint32_t &maxRpm)
{
    FanTachoSpec &rpmSpec{ *controller.getFanInfo(fanIndex).rpmSpec };
    rpmSpec.alertBelowRpm = minRpm;
    rpmSpec.alertAboveRpm = maxRpm;
    return true;
}


bool ConfigurationHook::updateFanTemperatureSensorIndex(uint8_t fanIndex, uint8_t tempSensorIndex)
{
    return controller.updateFanTemperatureSensorIndex(fanIndex, tempSensorIndex);
}


bool ConfigurationHook::updateTemperatureSensorAddress(uint8_t tempSensorIndex, const uint8_t (&address)[8])
{
    return controller.updateTemperatureSensorAddress(tempSensorIndex, address);
}


bool ConfigurationHook::setPowerCurvePoints(uint8_t fanIndex, const uint8_t (&power)[numCurvePoints], const int16_t (&tempDeciCelsius)[numCurvePoints])
{
    return controller.getFanInfo(fanIndex).interpolator.setPowerCurvePoints(power, tempDeciCelsius);
}


void ConfigurationHook::restoreRamFromSettings()
{
    autoreportDelayS = settings.getSettings().serialAutoreportSeconds;

    for(auto fanIdx : definedFanIndices)
    {
        FanInfo ram{ controller.getFanInfo(fanIdx) };
        FanSettings &storageSettings{ settings.getSettings().fans[fanIdx] };

        ram.pwmSpec->defaultDuty = storageSettings.defaultPwmDuty;
        ram.pwmSpec->errorDuty = storageSettings.errorPwmDuty;
        ram.pwmSpec->alertBelowDuty = storageSettings.alertBelowPwm;
        ram.pwmSpec->alertAboveDuty = storageSettings.alertAbovePwm;

        ram.rpmSpec->alertBelowRpm = storageSettings.alertBelowRpm;
        ram.rpmSpec->alertAboveRpm = storageSettings.alertAboveRpm;

        ram.fanTemperatureSpec->alertBelowTempC = 10.0f * static_cast<float>(storageSettings.alertBelowTempDeciC);
        ram.fanTemperatureSpec->alertAboveTempC = 10.0f * static_cast<float>(storageSettings.alertAboveTempDeciC);

        ram.interpolator.setPowerCurvePoints(storageSettings.fanCurvePower, storageSettings.fanCurveDeciCelsius);
    }

    for(auto sensorIdx : definedTemperatureSensorIndices)
        controller.updateTemperatureSensorAddress(sensorIdx, settings.getSettings().temperatureSensors[sensorIdx].address);
}

void ConfigurationHook::restoreSettingsFromRam()
{
    settings.getSettings().serialAutoreportSeconds = autoreportDelayS;

    for(auto fanIdx : definedFanIndices)
    {
        FanInfo ram{ controller.getFanInfo(fanIdx) };
        FanSettings &storageSettings{ settings.getSettings().fans[fanIdx] };

        storageSettings.defaultPwmDuty = ram.pwmSpec->defaultDuty;
        storageSettings.errorPwmDuty = ram.pwmSpec->errorDuty;
        storageSettings.alertBelowPwm = ram.pwmSpec->alertBelowDuty;
        storageSettings.alertAbovePwm = ram.pwmSpec->alertAboveDuty;

        storageSettings.alertBelowRpm = ram.rpmSpec->alertBelowRpm;
        storageSettings.alertAboveRpm = ram.rpmSpec->alertAboveRpm;

        storageSettings.alertBelowTempDeciC = static_cast<int16_t>(roundf(10.0f * ram.fanTemperatureSpec->alertBelowTempC));
        storageSettings.alertAboveTempDeciC = static_cast<int16_t>(roundf(10.0f * ram.fanTemperatureSpec->alertAboveTempC));

        const auto &powerCurve{ ram.interpolator.getCurve() };
        for(uint8_t idx{ 0 }; idx < numCurvePoints; idx++)
        {
            const auto &curvePoint{ powerCurve[idx] };
            storageSettings.fanCurvePower[idx] = curvePoint.power;
            storageSettings.fanCurveDeciCelsius[idx] = curvePoint.tempDeciCelsius;
        }
    }

    for(auto sensorIdx : definedTemperatureSensorIndices)
        controller.getTemperatureSensorAddress(sensorIdx, settings.getSettings().temperatureSensors[sensorIdx].address);
}
