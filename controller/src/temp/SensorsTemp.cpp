#include "SensorsTemp.h"


SensorsTemp::SensorsTemp(DallasTemperature &sensors) : sensors(sensors) {}


void SensorsTemp::begin()
{
    sensors.setResolution(TEMP_SENSORS_PRECISION_BITS);
    sensors.requestTemperatures();
}


bool SensorsTemp::readTemperatureCelsius(uint8_t idx, float &tempC)
{
    if(idx >= sizeof(addresses) / sizeof(DeviceAddress)) return false;

    tempC = sensors.getTempC(addresses[idx]);

    if(tempC <= DEVICE_DISCONNECTED_C) return false;
    else return true;
}
