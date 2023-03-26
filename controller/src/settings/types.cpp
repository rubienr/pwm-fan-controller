#include "types.h"
#include <Arduino.h>

const char *loadStatusToStr(LoadStatus t)
{
    switch(t)
    {
    case LoadStatus::Loaded:
        return "Loaded";
    case LoadStatus::OpenError:
        return "OpenError";
    case LoadStatus::ReadError:
        return "ReadError";
    case LoadStatus::CrcError:
        return "CrcError";
    case LoadStatus::VersionMismatch:
        return "VersionMismatch";
    default:
        return "-";
    }
}


const char *storeStatusToStr(StoreStatus t)
{
    switch(t)
    {
    case StoreStatus::Stored:
        return "Stored";
    case StoreStatus::OpenError:
        return "OpenError";
    case StoreStatus::WriteError:
        return "WriteError";
    default:
        return "-";
    }
}


void reportFanSettings(const FanSettings &f)
{
    Serial.print(millis());
    Serial.print(F(" #            defaultPwmDuty="));
    Serial.println(f.defaultPwmDuty);

    Serial.print(millis());
    Serial.print(F(" #            errorPwmDuty="));
    Serial.println(f.errorPwmDuty);

    Serial.print(millis());
    Serial.print(F(" #            temperatureSensorIndex="));
    Serial.println(f.temperatureSensorIndex);

    Serial.print(millis());
    Serial.print(F(" #            fanCurvePower={"));
    bool isFirst{ true };
    for(const auto &p : f.fanCurvePower)
    {
        if(!isFirst) Serial.print(F(", "));
        isFirst = false;
        Serial.print(p);
    }
    Serial.println(F("}"));

    Serial.print(millis());
    Serial.print(F(" #            fanCurveDeciCelsius={"));
    isFirst = true;
    for(const auto &c : f.fanCurveDeciCelsius)
    {
        if(!isFirst) Serial.print(F(", "));
        isFirst = false;
        Serial.print(c);
    }
    Serial.println(F("}"));

    Serial.print(millis());
    Serial.print(F(" #            alertBelowPwm="));
    Serial.println(f.alertBelowPwm);

    Serial.print(millis());
    Serial.print(F(" #            alertAbovePwm="));
    Serial.println(f.alertAbovePwm);

    Serial.print(millis());
    Serial.print(F(" #            alertBelowRpm="));
    Serial.println(f.alertBelowRpm);

    Serial.print(millis());
    Serial.print(F(" #            alertAboveRpm="));
    Serial.println(f.alertAboveRpm);

    Serial.print(millis());
    Serial.print(F(" #            alertBelowTempDeciC="));
    Serial.println(f.alertBelowTempDeciC);

    Serial.print(millis());
    Serial.print(F(" #            alertAboveTempDeciC="));
    Serial.println(f.alertAboveTempDeciC);
}


void reportTemperatureSensorSettings(const TemperatureSensorSettings &t)
{
    Serial.print(F("\""));
    bool isFirst{ true };
    for(const auto &a : t.address)
    {
        if(!isFirst) Serial.print(F(" "));
        isFirst = false;
        if(a <= 0xf) Serial.print(0);
        Serial.print(a, HEX);
    }
    Serial.print(F("\""));
}


void reportSettings(const Settings &s)
{
    Serial.print(millis());
    Serial.print(F(" #       serialAutoreportSeconds="));
    Serial.println(s.serialAutoreportSeconds);

    Serial.print(millis());
    Serial.println(F(" #       temperatureSensors:"));
    uint8_t addressIdx{ 0 };
    for(const auto &temperatureSensor : s.temperatureSensors)
    {
        Serial.print(millis());
        Serial.print(F(" #         address["));
        Serial.print(addressIdx++);
        Serial.print(F("]="));
        reportTemperatureSensorSettings(temperatureSensor);
        Serial.println();
    }

    Serial.print(millis());
    Serial.println(F(" #       fans:"));
    uint8_t fanIdx{ 0 };
    for(const auto &fan : s.fans)
    {
        Serial.print(millis());
        Serial.print(F(" #         fans["));
        Serial.print(fanIdx++);
        Serial.println(F("]:"));
        reportFanSettings(fan);
    }
}


void reportData(const Data &d)
{
    Serial.print(millis());
    Serial.print(F(" #     configWrites="));
    Serial.println(d.configWrites);
    reportVersion(d.version);
    Serial.print(millis());
    Serial.println(F(" #     Settings:"));
    reportSettings(d.settings);
}


void reportContainer(const Container &c)
{
    Serial.print(millis());
    Serial.println(F(" # Container:"));
    Serial.print(millis());
    Serial.print(F(" #   crc="));
    Serial.println(c.crc);
    Serial.print(millis());
    Serial.println(F(" #   Data:"));
    reportData(c.data);
}


void reportVersion(const Version &v)
{
    Serial.print(millis());
    Serial.println(F(" #     Version:"));
    Serial.print(millis());
    Serial.println(F(" #       versionNumber"));
    Serial.print(millis());
    Serial.print(F(" #         major="));
    Serial.println(v.versionNumber.major);
    Serial.print(millis());
    Serial.print(F(" #         minor="));
    Serial.println(v.versionNumber.minor);
    Serial.print(millis());
    Serial.print(F(" #         patch="));
    Serial.println(v.versionNumber.patch);
    Serial.print(millis());
    Serial.print(F(" #       buildTimestamp=\""));
    Serial.print(v.buildTimestamp);
    Serial.println(F("\""));
}


void TemperatureSensorSettings::reset(uint8_t tempSensorIndex)
{
    const uint8_t nullAddress[8]{ 0 };
    const uint8_t(&targetAddress)[8]{ isTemperatureSensorDefined(tempSensorIndex) ? definedTemperatureSensors[tempSensorIndex] : nullAddress };

    for(uint8_t idx{ 0 }; idx < 8; idx++)
        address[idx] = targetAddress[idx];
}

#define RESET_FAN_SETTINGS(n)                                                                      \
    {                                                                                              \
        defaultPwmDuty = FAN##n##_PWM_DEFAULT_DUTY;                                                \
        errorPwmDuty = FAN##n##_PWM_ERROR_DUTY;                                                    \
        temperatureSensorIndex = FAN##n##_TEMP_SENSOR_INDEX;                                       \
        constexpr const uint8_t defaultFanCurvePower[numCurvePoints] FAN##n##_CURVE_POWER;         \
        constexpr const int16_t defaultFanDeciCelsius[numCurvePoints] FAN##n##_CURVE_DECI_CELSIUS; \
        for(int8_t idx{ 0 }; idx < numCurvePoints; idx++)                                          \
        {                                                                                          \
            fanCurvePower[idx] = defaultFanCurvePower[idx];                                        \
            fanCurveDeciCelsius[idx] = defaultFanDeciCelsius[idx];                                 \
        }                                                                                          \
        alertBelowPwm = FAN##n##_ALERT_BELOW_PWM;                                                  \
        alertAbovePwm = FAN##n##_ALERT_ABOVE_PWM;                                                  \
        alertBelowRpm = FAN##n##_ALERT_BELOW_RPM;                                                  \
        alertAboveRpm = FAN##n##_ALERT_ABOVE_RPM;                                                  \
        alertBelowTempDeciC = FAN##n##_ALERT_BELOW_TEMP_DECI_CELSIUS;                              \
        alertAboveTempDeciC = FAN##n##_ALERT_ABOVE_TEMP_DECI_CELSIUS;                              \
    }

void FanSettings::reset(uint8_t fanIndex)
{
    switch(fanIndex)
    {
    case FAN0:
#if defined(FAN0)
        RESET_FAN_SETTINGS(0)
#endif
        return;
    case FAN1:
#if defined(FAN1)
        RESET_FAN_SETTINGS(1)
#endif
        return;
    case FAN2:
#if defined(FAN2)
        RESET_FAN_SETTINGS(2)
#endif
        return;
    case FAN3:
#if defined(FAN3)
        RESET_FAN_SETTINGS(3)
#endif
        return;
    case FAN4:
#if defined(FAN4)
        RESET_FAN_SETTINGS(4)
#endif
        return;
    default:
        return;
    }
}
