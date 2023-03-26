#include "TempSensors.h"


bool TempSensorSpec::hasAlert() const { return currentTempC < alertBelowTempC || currentTempC > alertAboveTempC; }


#define initTempSensorSpec(n)                                                                                           \
    {                                                                                                                   \
        sensors[FAN##n##_TEMP_SENSOR_INDEX].alertBelowTempC = 0.1f * FAN##n##_ALERT_BELOW_TEMP_DECI_CELSIUS;            \
        sensors[FAN##n##_TEMP_SENSOR_INDEX].alertAboveTempC = 0.1f * FAN##n##_ALERT_ABOVE_TEMP_DECI_CELSIUS;            \
        sensors[FAN##n##_TEMP_SENSOR_INDEX].sensorIndex = n;                                                            \
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


const TempSensorSpec &TempSensors::getSpecs(uint8_t tempSensorIndex) const { return sensors[tempSensorIndex]; }


TempSensorSpec &TempSensors::getSpecs(uint8_t tempSensorIndex) { return sensors[tempSensorIndex]; }


bool TempSensors::setDeviceAddress(uint8_t tempSensorIndex, const DeviceAddress &deviceAddress)
{
    if(!isTemperatureSensorDefined(tempSensorIndex)) return false;

    const uint8_t *address{ reinterpret_cast<const uint8_t *>(&deviceAddress) };
    for(size_t idx{ 0 }; idx < sizeof(DeviceAddress); idx++)
        sensors[tempSensorIndex].sensorAddress[idx] = address[idx];
    return true;
}
