#pragma once
#include "configuration.h"
#include <DallasTemperature.h>
#include <cinttypes>

struct TempSensorSpec
{
    [[nodiscard]] bool hasAlert() const; // true if temperature is not within alertBelowTempC and alertAboveTempC
    bool hasError{ false };              // true if temp sensor readout was erroneous
    float alertBelowTempC{ 0 };
    float alertAboveTempC{ 0 };
    DeviceAddress sensorAddress{ 0 };
    uint8_t sensorIndex{ 0 };
    float currentTempC{ 0 }; // last sensor conversion
};

struct TempSensors
{
    explicit TempSensors(DallasTemperature &sensors);
    void begin();
    void requestTemperatureConversion() const;
    bool fetchTemperatureCelsius(uint8_t idx);
    [[nodiscard]] const TempSensorSpec &getSpecs(uint8_t tempSensorIndex) const;
    [[nodiscard]] TempSensorSpec &getSpecs(uint8_t tempSensorIndex);
    bool setDeviceAddress(uint8_t tempSensorIndex, const DeviceAddress &address);

protected:
    DallasTemperature &sensorBus;
    TempSensorSpec sensors[getDefinedTemperatureSensorsCount()]{};
};
