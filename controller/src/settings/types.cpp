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


void reportSettings(const Settings &s)
{
    Serial.print(millis());
    Serial.print(" #       foo=");
    // TODO
    // Serial.println(s.foo);
}


void reportData(const Data &d)
{
    Serial.print(millis());
    Serial.print(" #     configWrites=");
    Serial.println(d.configWrites);
    reportVersion(d.version);
    Serial.print(millis());
    Serial.println(" #     Settings:");
    reportSettings(d.settings);
}


void reportContainer(const Container &c)
{
    Serial.print(millis());
    Serial.println(" # Container:");
    Serial.print(millis());
    Serial.print(" #   crc=");
    Serial.println(c.crc);
    Serial.print(millis());
    Serial.println(" #   Data:");
    reportData(c.data);
}


void reportVersion(const Version &v)
{
    Serial.print(millis());
    Serial.println(" #     Version:");
    Serial.print(millis());
    Serial.println(" #       versionNumber");
    Serial.print(millis());
    Serial.print(" #         major=");
    Serial.println(v.versionNumber.major);
    Serial.print(millis());
    Serial.print(" #         minor=");
    Serial.println(v.versionNumber.minor);
    Serial.print(millis());
    Serial.print(" #         patch=");
    Serial.println(v.versionNumber.patch);
    Serial.print(millis());
    Serial.print(" #       buildTimestamp=\"");
    Serial.print(v.buildTimestamp);
    Serial.println("\"");
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
