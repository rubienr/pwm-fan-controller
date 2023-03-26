#pragma once
#include "configuration.h"
#include <DallasTemperature.h>
#include <cinttypes>

struct TempSensorSpec
{
    bool hasError{ false }; // true if temp sensor readout was erroneous
    DeviceAddress sensorAddress{ 0 };
    float currentTempC{ 0 }; // last sensor conversion
};

struct TempSensors
{
    explicit TempSensors(DallasTemperature &sensors);
    void begin();
    void requestTemperatureConversion() const;
    bool fetchTemperatureCelsius(uint8_t idx);
    [[nodiscard]] const TempSensorSpec &getSpec(uint8_t tempSensorIndex) const;
    [[nodiscard]] TempSensorSpec &getSpec(uint8_t tempSensorIndex);
    bool setDeviceAddress(uint8_t tempSensorIndex, const DeviceAddress &address);
    bool getDeviceAddress(uint8_t tempSensorIndex, DeviceAddress &address);

protected:
    DallasTemperature &sensorBus;
    TempSensorSpec sensors[getDefinedTemperatureSensorsCount()]{};
};
