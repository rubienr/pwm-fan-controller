#include "OtaUpdate.h"
#if defined(OTA_UPDATE)
    #include <Arduino.h>
    #include <ArduinoOTA.h>
#endif

OtaUpdate::OtaUpdate(const char (&ssid)[OTA_WIFI_MAX_SSID_LENGTH], const char (&password)[OTA_WIFI_MAX_PASSWORD_LENGTH]) :
ssid(ssid), password(password)
{
}


bool OtaUpdate::enable()
{
#if defined(OTA_UPDATE)
    WiFiClass::mode(WIFI_STA);

    if(strlen(ssid) > 0 && strlen(password) > 0)
    {
        Serial.print(millis());
        Serial.print(F(" # connecting to wifi: "));
        Serial.println(ssid);
        WiFi.begin(ssid, password);
    }
    else
    {
        Serial.print(millis());
        Serial.println(F(" # connecting to access point using config from NVS memory ..."));
        WiFi.begin();
    }
    uint8_t connectRetries{ 6 };
    while(WiFiSTAClass::status() != WL_CONNECTED && connectRetries > 0)
    {
        delay(500);
        Serial.print(millis());
        Serial.print(F(" # wifi not connected, waiting=500ms, retries="));
        Serial.println(connectRetries);
        connectRetries--;
    }
    if(connectRetries == 0)
    {
        Serial.println(F(" # failed to connect wifi, giving up"));
        return false;
    }

    Serial.print(millis());
    Serial.print(" # wifi connected, signal strength: ");
    Serial.print(static_cast<int16_t>(WiFi.RSSI()));
    Serial.println(" dBm");
    Serial.print(millis());
    Serial.print(" # IP address: ");
    Serial.println(WiFi.localIP());

    ArduinoOTA.setRebootOnSuccess(true);
    ArduinoOTA.setMdnsEnabled(true);
    ArduinoOTA.setHostname(OTA_HOSTNAME);

    ArduinoOTA.onProgress(
    [](unsigned int a, unsigned int b)
    {
        Serial.print(millis());
        Serial.print(" # ");
        Serial.print(a);
        Serial.print("/");
        Serial.print(b);
        Serial.println(" bytes");
    });

    ArduinoOTA.setRebootOnSuccess(true);
    ArduinoOTA.setMdnsEnabled(true);
    ArduinoOTA.setHostname(OTA_HOSTNAME);
    ArduinoOTA.begin();

    Serial.print(millis());
    Serial.println(" # OTA enabled");
#endif
    return true;
}


void OtaUpdate::disable()
{
#if defined(OTA_UPDATE)
    WiFi.disconnect(true);
    Serial.print(millis());
    Serial.println(" # OTA disabled");
#endif
}


void OtaUpdate::process()
{
#if defined(OTA_UPDATE)
    ArduinoOTA.handle();
#endif
}
