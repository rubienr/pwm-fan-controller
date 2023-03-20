#pragma once
#include "configuration.h"
#include <DallasTemperature.h>


struct SensorsTemp
{
    explicit SensorsTemp(DallasTemperature &sensors);
    void begin();
    void captureTemperature();
    bool readTemperatureCelsius(uint8_t idx, float &tempC) const;

protected:
    DallasTemperature &sensors;
    DeviceAddress addresses [5] TEMP_SENSORS_ADDRESS;
};
