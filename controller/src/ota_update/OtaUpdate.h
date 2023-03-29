#pragma once
#include "configuration.h"

struct OtaUpdate
{
    OtaUpdate(const char (&ssid)[OTA_WIFI_MAX_SSID_LENGTH], const char (&password)[OTA_WIFI_MAX_PASSWORD_LENGTH]);
    bool enable();
    static void disable();
    static void process();

protected:
    const char (&ssid)[OTA_WIFI_MAX_SSID_LENGTH];
    const char (&password)[OTA_WIFI_MAX_PASSWORD_LENGTH];
};
