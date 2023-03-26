#include "FansTemperature.h"
#include "../sensors/TempSensors.h"

bool FanTemperatureSpec::hasAlert() const { return currentTempC < alertBelowTempC || currentTempC > alertAboveTempC; }


#define initTempSensorSpec(n)                                                             \
    {                                                                                     \
        fanTemps[FAN##n].alertBelowTempC = 0.1f * FAN##n##_ALERT_BELOW_TEMP_DECI_CELSIUS; \
        fanTemps[FAN##n].alertAboveTempC = 0.1f * FAN##n##_ALERT_ABOVE_TEMP_DECI_CELSIUS; \
        fanTemps[FAN##n].temperatureSensorIndex = n;                                      \
    }


FansTemperature::FansTemperature(TempSensors &temperatureSensors) : temperatureSensors(temperatureSensors)
{
#if defined(FAN0)
    initTempSensorSpec(0)
#endif
#if defined(FAN1)
    initTempSensorSpec(1)
#endif
#if defined(FAN2)
    initTempSensorSpec(2)
#endif
#if defined(FAN3)
    initTempSensorSpec(3)
#endif
#if defined(FAN4)
    initTempSensorSpec(4)
#endif
}


bool FansTemperature::fetchTemperatureCelsius(uint8_t fanIndex)
{
    if(!isFanDefined(fanIndex)) return false;
    const uint8_t temperatureSensorIndex{ fanTemps[fanIndex].temperatureSensorIndex };
    if(!isTemperatureSensorDefined(temperatureSensorIndex)) return false;
    fanTemps[fanIndex].currentTempC = temperatureSensors.getSpec(temperatureSensorIndex).currentTempC;
    fanTemps[fanIndex].hasError = temperatureSensors.getSpec(temperatureSensorIndex).hasError;
    return fanTemps[fanIndex].hasError;
}


const FanTemperatureSpec &FansTemperature::getSpec(uint8_t fanIndex) const { return fanTemps[fanIndex]; }


FanTemperatureSpec &FansTemperature::getSpec(uint8_t fanIndex) { return fanTemps[fanIndex]; }
