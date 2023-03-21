#include "TempSensors.h"


bool TempSensorSpecs::hasAlert() const { return currentTempC < alertBelowTempC || currentTempC > alertAboveTempC; }


TempSensors::TempSensors(DallasTemperature &sensorBus) : sensorBus(sensorBus)
{
    const DeviceAddress configuredAddresses[] TEMP_SENSORS_ADDRESS;
    constexpr size_t addressSize{ sizeof(DeviceAddress) };
    constexpr size_t numAddresses{ sizeof(configuredAddresses) / addressSize };
    static_assert(numAddresses >= 1 && numAddresses <= 5);

    if(numAddresses >= 1)
    {
        sensors[0].alertBelowTempC = FAN0_ALERT_BELOW_TEMP_C;
        sensors[0].alertAboveTempC = FAN0_ALERT_ABOVE_TEMP_C;
        sensors[0].sensorIndex = 0;
        memcpy(sensors[0].sensorAddress, configuredAddresses[0], addressSize);
    }
    if(numAddresses >= 2)
    {
        sensors[1].alertBelowTempC = FAN1_ALERT_BELOW_TEMP_C;
        sensors[1].alertAboveTempC = FAN1_ALERT_ABOVE_TEMP_C;
        sensors[1].sensorIndex = 1;
        memcpy(sensors[1].sensorAddress, configuredAddresses[1], addressSize);
    }
    if(numAddresses >= 3)
    {
        sensors[2].alertBelowTempC = FAN2_ALERT_BELOW_TEMP_C;
        sensors[2].alertAboveTempC = FAN2_ALERT_ABOVE_TEMP_C;
        sensors[2].sensorIndex = 2;
        memcpy(sensors[2].sensorAddress, configuredAddresses[2], addressSize);
    }
    if(numAddresses >= 4)
    {
        sensors[3].alertBelowTempC = FAN3_ALERT_BELOW_TEMP_C;
        sensors[3].alertAboveTempC = FAN3_ALERT_ABOVE_TEMP_C;
        sensors[3].sensorIndex = 3;
        memcpy(sensors[3].sensorAddress, configuredAddresses[3], addressSize);
    }
    if(numAddresses >= 5)
    {
        sensors[4].alertBelowTempC = FAN4_ALERT_BELOW_TEMP_C;
        sensors[4].alertAboveTempC = FAN4_ALERT_ABOVE_TEMP_C;
        sensors[4].sensorIndex = 4;
        memcpy(sensors[4].sensorAddress, configuredAddresses[4], addressSize);
    }
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

    TempSensorSpecs &info{ sensors[tempSensorIndex] };
    info.currentTempC = sensorBus.getTempC(info.sensorAddress);
    info.hasError = sensors[tempSensorIndex].currentTempC <= DEVICE_DISCONNECTED_C;
    return info.hasError;
}


void TempSensors::requestTemperatureConversion() const { sensorBus.requestTemperatures(); }


const TempSensorSpecs &TempSensors::getSpecs(uint8_t tempSensorIndex) const { return sensors[tempSensorIndex]; }


TempSensorSpecs &TempSensors::getSpecs(uint8_t tempSensorIndex) { return sensors[tempSensorIndex]; }
