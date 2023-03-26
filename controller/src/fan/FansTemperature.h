#pragma once
#include "configuration.h"
#include <DallasTemperature.h>
#include <cinttypes>

struct TempSensors;

struct FanTemperatureSpec
{
    [[nodiscard]] bool hasAlert() const; // true if temperature is not within alertBelowTempC and alertAboveTempC
    bool hasError{ false };              // true if temp sensor readout was erroneous
    float alertBelowTempC{ 0 };
    float alertAboveTempC{ 0 };
    uint8_t temperatureSensorIndex{ 0 };
    float currentTempC{ 0 }; // last sensor conversion
};

struct FansTemperature
{
    explicit FansTemperature(TempSensors &temperatureSensors);
    bool fetchTemperatureCelsius(uint8_t fanIndex);
    [[nodiscard]] const FanTemperatureSpec &getSpec(uint8_t fanIndex) const;
    [[nodiscard]] FanTemperatureSpec &getSpec(uint8_t fanIndex);

protected:
    TempSensors &temperatureSensors;
    FanTemperatureSpec fanTemps[getDefinedFansCount()]{};
};
