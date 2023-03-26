#pragma once
#include "../../lib/version/version.h"


enum class LoadStatus : uint8_t
{
    Loaded,
    OpenError,
    ReadError,
    CrcError,
    VersionMismatch,
};

const char *loadStatusToStr(LoadStatus t);


enum class StoreStatus : uint8_t
{
    Stored,
    OpenError,
    WriteError,
};

const char *storeStatusToStr(StoreStatus t);

struct FanSettings
{
    void reset(uint8_t fanIndex);

    uint32_t defaultPwmDuty{ 0 };
    uint32_t errorPwmDuty{ 0 };
    uint8_t temperatureSensorIndex{ 0 };
    uint8_t fanCurvePower[numCurvePoints]{ 0 };
    int16_t fanCurveDeciCelsius[numCurvePoints]{ 0 };
    uint32_t alertBelowPwm{ 0 };
    uint32_t alertAbovePwm{ 0 };
    uint32_t alertBelowRpm{ 0 };
    uint32_t alertAboveRpm{ 0 };
    int16_t alertBelowTempDeciC{ 0 };
    int16_t alertAboveTempDeciC{ 0 };
};


struct TemperatureSensorSettings
{
    void reset(uint8_t tempSensorIndex);
    uint8_t address[8]{ 0 };
};


template <uint8_t numFans, uint8_t numTemperatureSensors> struct SettingsMFansNSensors
{
    void reset()
    {
        serialAutoreportSeconds = SERIAL_AUTOREPORT_EVERY_SECONDS;

        for(const auto &idx : definedFanIndices)
            fans[idx].reset(idx);

        for(const auto &idx : definedTemperatureSensorIndices)
            temperatureSensors[idx].reset(idx);
    }

    unsigned long serialAutoreportSeconds
    {
#if defined(SERIAL_AUTOREPORT)
        SERIAL_AUTOREPORT_EVERY_SECONDS
#else
        0
#endif
    };
    FanSettings fans[numFans]{};
    TemperatureSensorSettings temperatureSensors[numTemperatureSensors]{};

} __attribute__((packed));


// always assume max fans/sensors to achieve same footprint in flash memory
using Settings = SettingsMFansNSensors<maxFansCount, maxTemperatureSensorsCount>;


void reportSettings(const Settings &s);


struct Data
{ // effective data to store
    uint32_t configWrites{ 0 };
    Version version{};
    Settings settings{};
} __attribute__((packed));


void reportVersion(const Version &v);


void reportData(const Data &d);


struct Container
{ // package sored/loaded to/from filesystem
    uint32_t crc{ 0 };
    Data data{};
} __attribute__((packed));


void reportContainer(const Container &c);
