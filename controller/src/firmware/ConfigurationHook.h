#pragma once

#include "../settings/types.h"
#include "controller/FansController.h"
#include "settings/FlashSettings.h"

struct ConfigurationHook
{
    ConfigurationHook(FansController &controller, FlashSettings &settings, unsigned long &autoreportDelayS);

    StoreStatus storeRamToFlash();
    LoadStatus restoreRamFromFlash();
    bool resetRamSettings();

    void reportSettings() const;
    unsigned long &getAutoreportDelaySeconds();
    bool setAutoreportDelaySeconds(const unsigned long &autoreportDelay);
    const FanInfo &getFanInfo(uint8_t tempSensorIndex) const { return controller.getFanInfo(tempSensorIndex); }
    bool setRpmAlert(uint8_t fanIndex, const uint32_t &minRpm, const uint32_t &maxRpPwm);
    bool setPwmAlert(uint8_t fanIndex, const uint32_t &minPwm, const uint32_t &maxPwm);
    bool setTemperatureAlert(uint8_t fanIndex, const int16_t &minTempDeciC, const int16_t &maxTempDeciCelsius);
    bool updateFanTemperatureSensorIndex(uint8_t fanIndex, uint8_t tempSensorIndex);
    bool updateTemperatureSensorAddress(uint8_t tempSensorIndex, const uint8_t (&address)[8]);
    bool setPowerCurvePoints(uint8_t fanIndex, const uint8_t (&power)[numCurvePoints], const int16_t (&tempDeciCelsius)[numCurvePoints]);

protected:
    void restoreRamFromSettings();
    void restoreSettingsFromRam();

    FansController &controller; // fan controller; contains effective settings
    FlashSettings &settings;    // storable settings; loaded from flash or ram; not effective settings
    unsigned long &autoreportDelayS;
};
