#include "ConfigurationHook.h"
#include "../fan/FansPwm.h"
#include "../fan/FansTacho.h"
#include "../fan/FansTemperature.h"
#include "../sensors/TempSensors.h"


StoreStatus ConfigurationHook::storeRamToFlash()
{
    writeRamToSettings();
    return settings.storeSettings();
}


LoadStatus ConfigurationHook::loadFlashToRam()
{
    const auto loadStatus{ settings.loadSettings() };
    if(LoadStatus::Loaded == loadStatus) writeSettingsToRam();
    return loadStatus;
}


bool ConfigurationHook::resetRamSettings()
{
    settings.reset();
    writeSettingsToRam();
    return true;
}


void ConfigurationHook::reportSettings() const { settings.reportContainer(); }


ConfigurationHook::ConfigurationHook(FansController &controller,
                                     FlashSettings &settings,
                                     unsigned long &autoreportDelayS,
                                     char (&ssid)[OTA_WIFI_MAX_SSID_LENGTH],
                                     char (&password)[OTA_WIFI_MAX_PASSWORD_LENGTH]) :
controller(controller),
settings(settings), autoreportDelayS(autoreportDelayS), wifiSsid(ssid), wifiPassword(password)
{
    // if(LoadStatus::Loaded != loadFlashToRam()) resetRamSettings();
    // loadFlashToRam();
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
    const float tempMinC{ static_cast<float>(minTempDeciC) * 0.1f }, tempMaxC{ static_cast<float>(maxTempDeciC) * 0.1f };
    fanTempSpec.alertBelowTempC = tempMinC;
    fanTempSpec.alertAboveTempC = tempMaxC;
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


bool ConfigurationHook::setWifiSsid(const char (&ssid)[OTA_WIFI_MAX_SSID_LENGTH])
{
    strcpy(wifiSsid, ssid);
    return true;
}


bool ConfigurationHook::getWifiSsid(char (&ssid)[OTA_WIFI_MAX_SSID_LENGTH])
{
    strcpy(ssid, wifiSsid);
    return true;
}


bool ConfigurationHook::setWifiPassword(const char (&password)[OTA_WIFI_MAX_PASSWORD_LENGTH])
{
    strcpy(wifiPassword, password);
    return true;
}


bool ConfigurationHook::getWifiPassword(char (&password)[OTA_WIFI_MAX_PASSWORD_LENGTH])
{
    strcpy(password, wifiPassword);
    return true;
}


void ConfigurationHook::writeSettingsToRam()
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

        ram.fanTemperatureSpec->alertBelowTempC = 0.1f * static_cast<float>(storageSettings.alertBelowTempDeciC);
        ram.fanTemperatureSpec->alertAboveTempC = 0.1f * static_cast<float>(storageSettings.alertAboveTempDeciC);
        ram.fanTemperatureSpec->temperatureSensorIndex = storageSettings.temperatureSensorIndex;

        ram.interpolator.setPowerCurvePoints(storageSettings.fanCurvePower, storageSettings.fanCurveDeciCelsius);
    }

    for(auto sensorIdx : definedTemperatureSensorIndices)
        controller.updateTemperatureSensorAddress(sensorIdx, settings.getSettings().temperatureSensors[sensorIdx].address);

    strcpy(wifiSsid, settings.getSettings().wifi.ssid);
    strcpy(wifiPassword, settings.getSettings().wifi.password);
}

void ConfigurationHook::writeRamToSettings()
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
        storageSettings.temperatureSensorIndex = ram.fanTemperatureSpec->temperatureSensorIndex;

        const auto &powerCurve{ ram.interpolator.getCurve() };
        for(uint8_t idx{ 0 }; idx < numCurvePoints; idx++)
        {
            const auto &ramCurvePoint{ powerCurve[idx] };
            storageSettings.fanCurvePower[idx] = ramCurvePoint.power;
            storageSettings.fanCurveDeciCelsius[idx] = ramCurvePoint.tempDeciCelsius;
        }
    }

    for(auto sensorIdx : definedTemperatureSensorIndices)
        controller.getTemperatureSensorAddress(sensorIdx, settings.getSettings().temperatureSensors[sensorIdx].address);

    strcpy(settings.getSettings().wifi.ssid, wifiSsid);
    strcpy(settings.getSettings().wifi.password, wifiPassword);
}
