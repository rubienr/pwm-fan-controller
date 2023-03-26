#pragma once

#include <cinttypes>

// ----- section fans

#if defined(FAN0)
    #undef FAN0
    #define FAN0 0
#endif

#if defined(FAN1)
    #undef FAN1
    #define FAN1 1
#endif

#if defined(FAN2)
    #undef FAN2
    #define FAN2 2
#endif

#if defined(FAN3)
    #undef FAN3
    #define FAN3 3
#endif

#if defined(FAN4)
    #undef FAN4
    #define FAN4 4
#endif

constexpr const bool isFan0Defined
{
#if defined(FAN0)
    true
#else
    false
#endif
};


constexpr const bool isFan1Defined
{
#if defined(FAN1)
    true
#else
    false
#endif
};


constexpr const bool isFan2Defined
{
#if defined(FAN2)
    true
#else
    false
#endif
};


constexpr const bool isFan3Defined
{
#if defined(FAN3)
    true
#else
    false
#endif
};


constexpr const bool isFan4Defined
{
#if defined(FAN4)
    true
#else
    false
#endif
};


constexpr bool isFanDefined(uint8_t fanIndex)
{
    switch(fanIndex)
    {
    case 0:
        return isFan0Defined;
    case 1:
        return isFan1Defined;
    case 2:
        return isFan2Defined;
    case 3:
        return isFan3Defined;
    case 4:
        return isFan4Defined;
    default:
        return false;
    }
}


constexpr bool hasFan0TemperatureSensorIndex(uint8_t fanTempSensorIndex)
{
    return
#if defined(FAN0)
    FAN0_TEMP_SENSOR_INDEX == fanTempSensorIndex;
#else
    false;
#endif
}


constexpr bool hasFan1TemperatureSensorIndex(uint8_t fanTempSensorIndex)
{
    return
#if defined(FAN1)
    FAN1_TEMP_SENSOR_INDEX == fanTempSensorIndex;
#else
    false;
#endif
}


constexpr bool hasFan2TemperatureSensorIndex(uint8_t fanTempSensorIndex)
{
    return
#if defined(FAN2)
    FAN2_TEMP_SENSOR_INDEX == fanTempSensorIndex;
#else
    false;
#endif
}


constexpr bool hasFan3TemperatureSensorIndex(uint8_t fanTempSensorIndex)
{
    return
#if defined(FAN3)
    FAN3_TEMP_SENSOR_INDEX == fanTempSensorIndex;
#else
    false;
#endif
}


constexpr bool hasFan4TemperatureSensorIndex(uint8_t fanTemperatureSensorIndex)
{
    return
#if defined(FAN4)
    FAN4_TEMP_SENSOR_INDEX == fanTemperatureSensorIndex;
#else
    false;
#endif
}

constexpr uint8_t maxFansCount{ 5 };


constexpr uint8_t getDefinedFansCount()
{
    uint8_t count{ 0 };
    count = (isFan0Defined) ? count + 1 : count;
    count = (isFan1Defined) ? count + 1 : count;
    count = (isFan2Defined) ? count + 1 : count;
    count = (isFan3Defined) ? count + 1 : count;
    count = (isFan4Defined) ? count + 1 : count;
    return count;
}


constexpr const uint8_t definedFanIndices[getDefinedFansCount()]
{
#if defined(FAN0)
    FAN0,
#endif
#if defined(FAN1)
    FAN1,
#endif
#if defined(FAN2)
    FAN2,
#endif
#if defined(FAN3)
    FAN3,
#endif
#if defined(FAN4)
    FAN4,
#endif
};

// ----- section temperature sensors

constexpr const bool isTemperatureSensor0Defined
{
#if defined(TEMPERATURE_SENSOR_0_ADDRESS)
    true
#else
    false
#endif
};


constexpr const bool isTemperatureSensor1Defined
{
#if defined(TEMPERATURE_SENSOR_1_ADDRESS)
    true
#else
    false
#endif
};


constexpr const bool isTemperatureSensor2Defined
{
#if defined(TEMPERATURE_SENSOR_2_ADDRESS)
    true
#else
    false
#endif
};


constexpr const bool isTemperatureSensor3Defined
{
#if defined(TEMPERATURE_SENSOR_3_ADDRESS)
    true
#else
    false
#endif
};


constexpr const bool isTemperatureSensor4Defined
{
#if defined(TEMPERATURE_SENSOR_4_ADDRESS)
    true
#else
    false
#endif
};

constexpr bool isTemperatureSensorDefined(uint8_t tempSensorIndex)
{
    switch(tempSensorIndex)
    {
    case 0:
        return isTemperatureSensor0Defined;
    case 1:
        return isTemperatureSensor1Defined;
    case 2:
        return isTemperatureSensor2Defined;
    case 3:
        return isTemperatureSensor3Defined;
    case 4:
        return isTemperatureSensor4Defined;
    default:
        return false;
    }
}

constexpr uint8_t maxTemperatureSensorsCount{ 5 };

constexpr uint8_t getDefinedTemperatureSensorsCount()
{
    uint8_t count{ 0 };
    count = (isTemperatureSensor0Defined) ? count + 1 : count;
    count = (isTemperatureSensor1Defined) ? count + 1 : count;
    count = (isTemperatureSensor2Defined) ? count + 1 : count;
    count = (isTemperatureSensor3Defined) ? count + 1 : count;
    count = (isTemperatureSensor4Defined) ? count + 1 : count;
    return count;
}


constexpr const uint8_t definedTemperatureSensorIndices[getDefinedTemperatureSensorsCount()]
{
#if defined(TEMPERATURE_SENSOR_0_ADDRESS)
    0,
#endif
#if defined(TEMPERATURE_SENSOR_1_ADDRESS)
    1,
#endif
#if defined(TEMPERATURE_SENSOR_2_ADDRESS)
    2,
#endif
#if defined(TEMPERATURE_SENSOR_3_ADDRESS)
    3,
#endif
#if defined(TEMPERATURE_SENSOR_4_ADDRESS)
    4,
#endif
};


constexpr const uint8_t definedTemperatureSensors[getDefinedTemperatureSensorsCount()][8]
{
#if defined(TEMPERATURE_SENSOR_0_ADDRESS)
    TEMPERATURE_SENSOR_0_ADDRESS,
#endif
#if defined(TEMPERATURE_SENSOR_1_ADDRESS)
    TEMPERATURE_SENSOR_1_ADDRESS,
#endif
#if defined(TEMPERATURE_SENSOR_2_ADDRESS)
    TEMPERATURE_SENSOR_2_ADDRESS,
#endif
#if defined(TEMPERATURE_SENSOR_3_ADDRESS)
    TEMPERATURE_SENSOR_3_ADDRESS,
#endif
#if defined(TEMPERATURE_SENSOR_4_ADDRESS)
    TEMPERATURE_SENSOR_4_ADDRESS,
#endif
};

// ----- section power curve

constexpr const uint8_t numCurvePoints{ 4 };
