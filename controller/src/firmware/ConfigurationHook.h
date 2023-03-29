#pragma once

#include "../settings/types.h"
#include "controller/FansController.h"
#include "settings/FlashSettings.h"

struct ConfigurationHook
{
    ConfigurationHook(FansController &controller,
                      FlashSettings &settings,
                      unsigned long &autoreportDelayS,
                      char (&ssid)[OTA_WIFI_MAX_SSID_LENGTH],
                      char (&password)[OTA_WIFI_MAX_PASSWORD_LENGTH]);

    StoreStatus storeRamToFlash();
    LoadStatus loadFlashToRam();
    bool resetRamSettings();

    void reportSettings() const;
    const unsigned long &getAutoreportDelaySeconds();
    bool setAutoreportDelaySeconds(const unsigned long &autoreportDelay);
    [[nodiscard]] const FanInfo &getFanInfo(uint8_t tempSensorIndex) const { return controller.getFanInfo(tempSensorIndex); }
    bool setWifiSsid(const char (&ssid)[OTA_WIFI_MAX_SSID_LENGTH]);
    bool getWifiSsid(char (&ssid)[OTA_WIFI_MAX_SSID_LENGTH]);
    bool setWifiPassword(const char (&password)[OTA_WIFI_MAX_PASSWORD_LENGTH]);
    bool getWifiPassword(char (&password)[OTA_WIFI_MAX_PASSWORD_LENGTH]);
    bool setRpmAlert(uint8_t fanIndex, const uint32_t &minRpm, const uint32_t &maxRpPwm);
    bool setPwmAlert(uint8_t fanIndex, const uint32_t &minPwm, const uint32_t &maxPwm);
    bool setTemperatureAlert(uint8_t fanIndex, const int16_t &minTempDeciC, const int16_t &maxTempDeciCelsius);
    bool updateFanTemperatureSensorIndex(uint8_t fanIndex, uint8_t tempSensorIndex);
    bool updateTemperatureSensorAddress(uint8_t tempSensorIndex, const uint8_t (&address)[8]);
    bool setPowerCurvePoints(uint8_t fanIndex, const uint8_t (&power)[numCurvePoints], const int16_t (&tempDeciCelsius)[numCurvePoints]);

protected:
    void writeSettingsToRam();
    void writeRamToSettings();

    FansController &controller; // fan controller; contains effective (ram) settings
    FlashSettings &settings;    // flash-storable settings; loaded from flash or ram; not effective settings
    unsigned long &autoreportDelayS;
    char (&wifiSsid)[OTA_WIFI_MAX_SSID_LENGTH];
    char (&wifiPassword)[OTA_WIFI_MAX_PASSWORD_LENGTH];
};
