#include "TempSensors.h"


#define initTempSensorSpec(n)                                                                                           \
    {                                                                                                                   \
        memcpy(sensors[FAN##n##_TEMP_SENSOR_INDEX].sensorAddress, definedTemperatureSensors[n], sizeof(DeviceAddress)); \
    }


TempSensors::TempSensors(DallasTemperature &sensorBus) : sensorBus(sensorBus)
{
    if(isTemperatureSensor0Defined) initTempSensorSpec(0);
    if(isTemperatureSensor1Defined) initTempSensorSpec(0);
    if(isTemperatureSensor2Defined) initTempSensorSpec(0);
    if(isTemperatureSensor3Defined) initTempSensorSpec(0);
    if(isTemperatureSensor4Defined) initTempSensorSpec(0);
}


void TempSensors::begin()
{
    sensorBus.setResolution(TEMPERATURE_SENSORS_PRECISION_BITS);
    sensorBus.requestTemperatures();
}


bool TempSensors::fetchTemperatureCelsius(uint8_t tempSensorIndex)
{
    if(!isTemperatureSensorDefined(tempSensorIndex)) return false;

    TempSensorSpec &info{ sensors[tempSensorIndex] };
    info.currentTempC = sensorBus.getTempC(info.sensorAddress);
    info.hasError = sensors[tempSensorIndex].currentTempC <= DEVICE_DISCONNECTED_C;
    return info.hasError;
}


void TempSensors::requestTemperatureConversion() const { sensorBus.requestTemperatures(); }


const TempSensorSpec &TempSensors::getSpec(uint8_t tempSensorIndex) const { return sensors[tempSensorIndex]; }


TempSensorSpec &TempSensors::getSpec(uint8_t tempSensorIndex) { return sensors[tempSensorIndex]; }


bool TempSensors::setDeviceAddress(uint8_t tempSensorIndex, const DeviceAddress &deviceAddress)
{
    if(!isTemperatureSensorDefined(tempSensorIndex)) return false;
    memcpy(sensors[tempSensorIndex].sensorAddress, &deviceAddress, sizeof(DeviceAddress));
    return true;
}


bool TempSensors::getDeviceAddress(uint8_t tempSensorIndex, DeviceAddress &deviceAddress)
{
    if(!isTemperatureSensorDefined(tempSensorIndex)) return false;
    memcpy(&deviceAddress, sensors[tempSensorIndex].sensorAddress, sizeof(DeviceAddress));
    return true;
}
