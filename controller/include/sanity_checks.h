#pragma once

static_assert(getDefinedFansCount() >= 1);
static_assert(getDefinedFansCount() <= 5);

static_assert(getDefinedTemperatureSensorsCount() >= 1);
static_assert(getDefinedTemperatureSensorsCount() <= 5);

#if defined FAN4
    #if defined FAN3
        #if defined FAN2
            #if defined FAN1
                #if defined FAN0
                // ok
                #else
static_assert(false, "define FAN0 before FAN1");
                #endif
            #else
static_assert(false, "define FAN1 before FAN2");
            #endif
        #else
static_assert(false, "define FAN2 before FAN3");
        #endif
    #else
static_assert(false, "define FAN3 before FAN4");
    #endif
#endif


#if defined TEMPERATURE_SENSOR_4_ADDRESS
    #if defined TEMPERATURE_SENSOR_3_ADDRESS
        #if defined TEMPERATURE_SENSOR_2_ADDRESS
            #if defined TEMPERATURE_SENSOR_1_ADDRESS
                #if defined TEMPERATURE_SENSOR_0_ADDRESS
                // ok
                #else
static_assert(false, "define sensor TEMPERATURE_SENSOR_0_ADDRESS before TEMPERATURE_SENSOR_1_ADDRESS");
                #endif
            #else
static_assert(false, "define sensor TEMPERATURE_SENSOR_1_ADDRESS before TEMPERATURE_SENSOR_2_ADDRESS");
            #endif
        #else
static_assert(false, "define sensor TEMPERATURE_SENSOR_2_ADDRESS before TEMPERATURE_SENSOR_3_ADDRESS");
        #endif
    #else
static_assert(false, "define sensor TEMPERATURE_SENSOR_3_ADDRESS before TEMPERATURE_SENSOR_4_ADDRESS");
    #endif
#endif
