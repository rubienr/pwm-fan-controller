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
    Serial.println(s.foo);
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
