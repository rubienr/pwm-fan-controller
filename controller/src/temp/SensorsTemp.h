#pragma once
#include "configuration.h"
#include <DallasTemperature.h>


struct SensorsTemp
{
    explicit SensorsTemp(DallasTemperature &sensors);
    void begin();
    void captureTemperature() {
        sensors.requestTemperatures();
    }
    bool readTemperatureCelsius(uint8_t idx, float &tempC);

protected:
    DallasTemperature &sensors;
    DeviceAddress addresses [5] TEMP_SENSORS_ADDRESS;
};
