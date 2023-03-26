#include "ConfigurationHook.h"
#include "../fan/FansPwm.h"
#include "../fan/FansTacho.h"
#include "../temp/TempSensors.h"

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


unsigned long &ConfigurationHook::getAutoreportDelaySeconds() { return autoreportDelayS; }


bool ConfigurationHook::setPwmAlert(uint8_t fanIndex, const uint32_t &minPwm, const uint32_t &maxPwm)
{
    FanPwmSpec &pwmInfo{ *controller.getFanInfo(fanIndex).pwmSpecs };
    pwmInfo.alertBelowDuty = minPwm;
    pwmInfo.alertAboveDuty = maxPwm;
    return true;
}


bool ConfigurationHook::setTemperatureAlert(uint8_t fanIndex, const int16_t &minTempDeciC, const int16_t &maxTempDeciC)
{
    TempSensorSpec &tempInfo{ *controller.getFanInfo(fanIndex).tempSpecs };
    const float tempMin{ static_cast<float>(minTempDeciC) * 10.0f }, tempMax{ static_cast<float>(maxTempDeciC) * 10.0f };
    tempInfo.alertBelowTempC = tempMin;
    tempInfo.alertAboveTempC = tempMax;
    return true;
}


bool ConfigurationHook::setRpmAlert(uint8_t fanIndex, const uint32_t &minRpm, const uint32_t &maxRpm)
{
    FanTachoSpec &rpmInfo{ *controller.getFanInfo(fanIndex).rpmSpecs };
    rpmInfo.alertBelowRpm = minRpm;
    rpmInfo.alertAboveRpm = maxRpm;
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
    // todo
}

void ConfigurationHook::restoreSettingsFromRam()
{
    // todo
}
