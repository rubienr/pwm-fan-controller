#include "TempSensors.h"


bool TempSensorSpec::hasAlert() const { return currentTempC < alertBelowTempC || currentTempC > alertAboveTempC; }


#define initTempSensorSpec(n)                                                                                     \
    {                                                                                                             \
        sensors[FAN##n##_TEMP_SENSOR_INDEX].alertBelowTempC = FAN##n##_ALERT_BELOW_TEMP_C;                        \
        sensors[FAN##n##_TEMP_SENSOR_INDEX].alertAboveTempC = FAN##n##_ALERT_ABOVE_TEMP_C;                        \
        sensors[FAN##n##_TEMP_SENSOR_INDEX].sensorIndex = n;                                                      \
        memcpy(sensors[FAN##n##_TEMP_SENSOR_INDEX].sensorAddress, configuredAddresses[n], sizeof(DeviceAddress)); \
    }


TempSensors::TempSensors(DallasTemperature &sensorBus) : sensorBus(sensorBus)
{
    const DeviceAddress configuredAddresses[] TEMP_SENSORS_ADDRESS;
    static_assert(getDefinedTempSensorsCount() >= 1 && getDefinedTempSensorsCount() <= 5);

    if(getDefinedTempSensorsCount() > 0) initTempSensorSpec(0);
    if(getDefinedTempSensorsCount() > 1) initTempSensorSpec(1);
    if(getDefinedTempSensorsCount() > 2) initTempSensorSpec(2);
    if(getDefinedTempSensorsCount() > 3) initTempSensorSpec(3);
    if(getDefinedTempSensorsCount() > 4) initTempSensorSpec(4);
}


void TempSensors::begin()
{
    sensorBus.setResolution(TEMP_SENSORS_PRECISION_BITS);
    sensorBus.requestTemperatures();
}


bool TempSensors::fetchTemperatureCelsius(uint8_t tempSensorIndex)
{
    const DeviceAddress configuredAddresses[] TEMP_SENSORS_ADDRESS;
    constexpr size_t addressSize{ sizeof(DeviceAddress) };
    constexpr size_t numAddresses{ sizeof(configuredAddresses) / addressSize };
    if(tempSensorIndex >= numAddresses) return false;

    TempSensorSpec &info{ sensors[tempSensorIndex] };
    info.currentTempC = sensorBus.getTempC(info.sensorAddress);
    info.hasError = sensors[tempSensorIndex].currentTempC <= DEVICE_DISCONNECTED_C;
    return info.hasError;
}


void TempSensors::requestTemperatureConversion() const { sensorBus.requestTemperatures(); }


const TempSensorSpec &TempSensors::getSpecs(uint8_t tempSensorIndex) const { return sensors[tempSensorIndex]; }


TempSensorSpec &TempSensors::getSpecs(uint8_t tempSensorIndex) { return sensors[tempSensorIndex]; }
