#pragma once
#include "Resources.h"

struct Firmware : public Resources
{
    void setup();
    void process();

protected:
    static char getTrendSymbol(const FanInfo &info);
    void displayFan(uint8_t fanIndex);
    void displayFansInfo();
    static void reportAddress(const DeviceAddress &deviceAddress);
    void reportTemperatureSensor(uint8_t sensorIndex, const DeviceAddress &address);
    static void reportFanInfo(uint8_t fanIndex, const FanInfo &info, bool reportOnError = true, bool includeTimeStamp = true);
    void reportFansInfo() const;
};
