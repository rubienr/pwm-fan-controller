#pragma once
#include <cinttypes>

// ----- section: version

#define VERSION_MAJOR                 0
#define VERSION_MINOR                 0
#define VERSION_PATCH                 2

// ----- section serial

#define SERIAL_BAUD                   115200
#define SERIAL_CONFIG                 SERIAL_8N1
#define SERIAL_ECHO_ON                // echoes received characters back to serial console, comment to disable

// ----- section screen

#define SCREEN_WIDTH_PX               128
#define SCREEN_HEIGHT_PX              64
#define SCREEN_ADDRESS                0x3C
#define SCREEN_WIRE_SDA               5
#define SCREEN_WIRE_SCL               4

// ----- section flash settings

#define FLASH_SETTINGS_FILE_PATH_NAME "/settings.cfg"

// ----- section fan

#define FAN0
#if defined(FAN0)
    #define FAN0_INDEX                 0                              // 0 - 6, must be unique
    #define FAN0_TACHO_GPIO_NUM        36                             // tacho input pin
    #define FAN0_TACHO_COUNTER_UNIT    pcnt_unit_t::PCNT_UNIT_0       // signal counter unit
    #define FAN0_TACHO_COUNTER_CHANNEL pcnt_channel_t::PCNT_CHANNEL_0 // signal counter unit-channel

    #define FAN0_PWM_GPIO_NUM          25    // PWM output pin
    #define FAN0_PWM_CHANNEL           0     // PWM channel
    #define FAN0_PWM_FREQUENCY_HZ      25000 // recommended 25kHz
    #define FAN0_PWM_RESOLUTION_BITS   8     // recommended 8bit
    #define FAN0_PWM_MIN_DUTY          0     // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN0_PWM_MAX_DUTY          255   // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN0_PWM_DEFAULT_DUTY      100   // duty on startup w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN0_PWM_ERROR_DUTY        255   // duty on error w.r.t. FANx_PWM_RESOLUTION_BITS

    #define FAN0_TEMP_SENSOR_INDEX     0 // temperature sensor this fan depends on
    // clang-format off
    #define FAN0_CURVE_POWER         {50,  50, 100, 100} // target power, 0-255
    #define FAN0_CURVE_CENTI_CELSIUS { 0, 250, 700, 800} // input temperature; i.e. 123 corresponds to 12,34°C
// clang-format on

    #define FAN0_ALERT_BELOW_PWM       50   // trigger alert below PWM threshold
    #define FAN0_ALERT_ABOVE_PWM       50   // trigger alert above PWM threshold
    #define FAN0_ALERT_BELOW_RPM       0    // trigger alert below RPM threshold
    #define FAN0_ALERT_ABOVE_RPM       1500 // trigger alert above RPM threshold
    #define FAN0_ALERT_BELOW_TEMP_C    0.0  // trigger alert below temperature threshold
    #define FAN0_ALERT_ABOVE_TEMP_C    60.0 // trigger alert above temperature threshold

#endif

#define FAN1
#if defined(FAN1)
    #define FAN1_INDEX                 1                              // 0 - 6, must be unique
    #define FAN1_TACHO_GPIO_NUM        39                             // tacho input pin
    #define FAN1_TACHO_COUNTER_UNIT    pcnt_unit_t::PCNT_UNIT_1       // signal counter unit
    #define FAN1_TACHO_COUNTER_CHANNEL pcnt_channel_t::PCNT_CHANNEL_0 // signal counter unit-channel

    #define FAN1_PWM_GPIO_NUM          16    // PWM output pin
    #define FAN1_PWM_CHANNEL           1     // PWM channel
    #define FAN1_PWM_FREQUENCY_HZ      25000 // recommended 25kHz
    #define FAN1_PWM_RESOLUTION_BITS   8     // recommended 8bit
    #define FAN1_PWM_MIN_DUTY          0     // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN1_PWM_MAX_DUTY          255   // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN1_PWM_DEFAULT_DUTY      100   // duty on startup w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN1_PWM_ERROR_DUTY        255   // duty on error w.r.t. FANx_PWM_RESOLUTION_BITS

    #define FAN1_TEMP_SENSOR_INDEX     1 // temperature sensor this fan depends on

    // clang-format off
    #define FAN1_CURVE_POWER         {50,  50, 100, 100} // target power, 0-255
    #define FAN1_CURVE_CENTI_CELSIUS { 0, 250, 700, 800} // input temperature; i.e. 123 corresponds to 12,34°C
// clang-format on

    #define FAN1_ALERT_BELOW_PWM       1    // trigger alert below PWM threshold
    #define FAN1_ALERT_ABOVE_PWM       254  // trigger alert above PWM threshold
    #define FAN1_ALERT_BELOW_RPM       0    // trigger alert below RPM threshold
    #define FAN1_ALERT_ABOVE_RPM       1500 // trigger alert above RPM threshold
    #define FAN1_ALERT_BELOW_TEMP_C    0.0  // trigger alert below temperature threshold
    #define FAN1_ALERT_ABOVE_TEMP_C    60.0 // trigger alert above temperature threshold
#endif

#define FAN2
#if defined(FAN2)
    #define FAN2_INDEX                 2                              // 0 - 6, must be unique
    #define FAN2_TACHO_GPIO_NUM        0                              // tacho input pin
    #define FAN2_TACHO_COUNTER_UNIT    pcnt_unit_t::PCNT_UNIT_2       // signal counter unit
    #define FAN2_TACHO_COUNTER_CHANNEL pcnt_channel_t::PCNT_CHANNEL_0 // signal counter unit-channel

    #define FAN2_PWM_GPIO_NUM          2     // PWM output pin
    #define FAN2_PWM_CHANNEL           2     // PWM channel
    #define FAN2_PWM_FREQUENCY_HZ      25000 // recommended 25kHz
    #define FAN2_PWM_RESOLUTION_BITS   8     // recommended 8bit
    #define FAN2_PWM_MIN_DUTY          0     // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN2_PWM_MAX_DUTY          255   // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN2_PWM_DEFAULT_DUTY      100   // duty on startup w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN2_PWM_ERROR_DUTY        255   // duty on error w.r.t. FANx_PWM_RESOLUTION_BITS

    #define FAN2_TEMP_SENSOR_INDEX     2 // temperature sensor this fan depends on

    // clang-format off
    #define FAN2_CURVE_POWER         {50,  50, 100, 100} // target power, 0-255
    #define FAN2_CURVE_CENTI_CELSIUS { 0, 250, 700, 800} // input temperature; i.e. 123 corresponds to 12,34°C
// clang-format on

    #define FAN2_ALERT_BELOW_PWM       1    // trigger alert below PWM threshold
    #define FAN2_ALERT_ABOVE_PWM       254  // trigger alert above PWM threshold
    #define FAN2_ALERT_BELOW_RPM       0    // trigger alert below RPM threshold
    #define FAN2_ALERT_ABOVE_RPM       1500 // trigger alert above RPM threshold
    #define FAN2_ALERT_BELOW_TEMP_C    0.0  // trigger alert below temperature threshold
    #define FAN2_ALERT_ABOVE_TEMP_C    60.0 // trigger alert above temperature threshold
#endif

#define FAN3
#if defined(FAN3)
    #define FAN3_INDEX                 3                              // 0 - 6, must be unique
    #define FAN3_TACHO_GPIO_NUM        14                             // tacho input pin
    #define FAN3_TACHO_COUNTER_UNIT    pcnt_unit_t::PCNT_UNIT_3       // signal counter unit
    #define FAN3_TACHO_COUNTER_CHANNEL pcnt_channel_t::PCNT_CHANNEL_0 // signal counter unit-channel

    #define FAN3_PWM_GPIO_NUM          12    // PWM output pin
    #define FAN3_PWM_CHANNEL           3     // PWM channel
    #define FAN3_PWM_FREQUENCY_HZ      25000 // recommended 25kHz
    #define FAN3_PWM_RESOLUTION_BITS   8     // recommended 8bit
    #define FAN3_PWM_MIN_DUTY          0     // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN3_PWM_MAX_DUTY          255   // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN3_PWM_DEFAULT_DUTY      100   // duty on startup w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN3_PWM_ERROR_DUTY        255   // duty on error w.r.t. FANx_PWM_RESOLUTION_BITS

    #define FAN3_TEMP_SENSOR_INDEX     3 // temperature sensor this fan depends on

    // clang-format off
    #define FAN3_CURVE_POWER         {50,  50, 100, 100} // target power, 0-255
    #define FAN3_CURVE_CENTI_CELSIUS { 0, 250, 700, 800} // input temperature; i.e. 123 corresponds to 12,34°C
// clang-format on

    #define FAN3_ALERT_BELOW_PWM       1    // trigger alert below PWM threshold
    #define FAN3_ALERT_ABOVE_PWM       254  // trigger alert above PWM threshold
    #define FAN3_ALERT_BELOW_RPM       0    // trigger alert below RPM threshold
    #define FAN3_ALERT_ABOVE_RPM       1500 // trigger alert above RPM threshold
    #define FAN3_ALERT_BELOW_TEMP_C    0.0  // trigger alert below temperature threshold
    #define FAN3_ALERT_ABOVE_TEMP_C    60.0 // trigger alert above temperature threshold
#endif

#define FAN4
#if defined(FAN4)
    #define FAN4_INDEX                 4                              // 0 - 6, must be unique
    #define FAN4_TACHO_GPIO_NUM        13                             // tacho input pin
    #define FAN4_TACHO_COUNTER_UNIT    pcnt_unit_t::PCNT_UNIT_4       // signal counter unit
    #define FAN4_TACHO_COUNTER_CHANNEL pcnt_channel_t::PCNT_CHANNEL_0 // signal counter unit-channel

    #define FAN4_PWM_GPIO_NUM          15    // PWM output pin
    #define FAN4_PWM_CHANNEL           4     // PWM channel
    #define FAN4_PWM_FREQUENCY_HZ      25000 // recommended 25kHz
    #define FAN4_PWM_RESOLUTION_BITS   8     // recommended 8bit
    #define FAN4_PWM_MIN_DUTY          0     // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN4_PWM_MAX_DUTY          255   // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN4_PWM_DEFAULT_DUTY      100   // duty on startup w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN4_PWM_ERROR_DUTY        255   // duty on error w.r.t. FANx_PWM_RESOLUTION_BITS

    #define FAN4_TEMP_SENSOR_INDEX     4 // temperature sensor this fan depends on

    // clang-format off
    #define FAN4_CURVE_POWER         {50,  50, 100, 100} // target power, 0-255
    #define FAN4_CURVE_CENTI_CELSIUS { 0, 250, 700, 800} // input temperature; i.e. 123 corresponds to 12,34°C
// clang-format on

    #define FAN4_ALERT_BELOW_PWM       1    // trigger alert below PWM threshold
    #define FAN4_ALERT_ABOVE_PWM       254  // trigger alert above PWM threshold
    #define FAN4_ALERT_BELOW_RPM       0    // trigger alert below RPM threshold
    #define FAN4_ALERT_ABOVE_RPM       1500 // trigger alert above RPM threshold
    #define FAN4_ALERT_BELOW_TEMP_C    0.0  // trigger alert below temperature threshold
    #define FAN4_ALERT_ABOVE_TEMP_C    60.0 // trigger alert above temperature threshold

#endif

// ----- section temperature sensors (DS18xxx)

#define TEMP_ONE_WIRE_GPIO_NUM      26 // one wire pin
#define TEMP_SENSORS_PRECISION_BITS 10 // 9-12 bits; conversion time: 9-bit about 94ms, 12-bit about 750ms

// clang-format off
// List of up to 5 sensors that matches one or multiple fan(s) by index FANx_INDEX.
#define TEMP_SENSORS_ADDRESS                                \
    {                                                       \
        { 0x28, 0xFF, 0x64, 0x02, 0x19, 0x86, 0x14, 0xFC }, \
        { 0x28, 0xD9, 0xEC, 0x09, 0x06, 0x00, 0x00, 0xF5 }, \
        { 0x28, 0xFF, 0x64, 0x02, 0x19, 0x8B, 0xD0, 0xDE }, \
        { 0x28, 0x64, 0x41, 0x0A, 0x06, 0x00, 0x00, 0xEF }, \
        { 0x28, 0x05, 0x52, 0x08, 0x06, 0x00, 0x00, 0x2E }, \
    }
// clang-format on

// ----- section logging

#define SERIAL_AUTOREPORT           // enable auto reporting to serial console
#if defined(SERIAL_AUTOREPORT)
    #define SERIAL_AUTOREPORT_EVERY_SECONDS 0 // 0=disabled, reports the fan status information every N seconds
#endif


// ----- plausibility checks helpers

const bool isFan0Defined
{
#if defined(FAN0)
    true
#else
    false
#endif
};

const bool isFan1Defined
{
#if defined(FAN1)
    true
#else
    false
#endif
};

const bool isFan2Defined
{
#if defined(FAN2)
    true
#else
    false
#endif
};

const bool isFan3Defined
{
#if defined(FAN3)
    true
#else
    false
#endif
};

const bool isFan4Defined
{
#if defined(FAN4)
    true
#else
    false
#endif
};


bool constexpr isFan0Index(uint8_t fanIndex)
{
    return
#if defined(FAN0_INDEX)
    FAN0_INDEX == fanIndex;
#else
    false;
#endif
}


bool constexpr isFan1Index(uint8_t fanIndex)
{
    return
#if defined(FAN1_INDEX)
    FAN1_INDEX == fanIndex;
#else
    false;
#endif
}


bool constexpr isFan2Index(uint8_t fanIndex)
{
    return
#if defined(FAN2_INDEX)
    FAN2_INDEX == fanIndex;
#else
    false;
#endif
}


bool constexpr isFan3Index(uint8_t fanIndex)
{
    return
#if defined(FAN3_INDEX)
    FAN3_INDEX == fanIndex;
#else
    false;
#endif
}


bool constexpr isFan4Index(uint8_t fanIndex)
{
    return
#if defined(FAN4_INDEX)
    FAN4_INDEX == fanIndex;
#else
    false;
#endif
}


bool constexpr isFan0TempSensorIndex(uint8_t fanTempSensorIndex)
{
    return
#if defined(FAN0_INDEX)
    FAN0_INDEX == fanTempSensorIndex;
#else
    false;
#endif
}


bool constexpr isFan1TempSensorIndex(uint8_t fanTempSensorIndex)
{
    return
#if defined(FAN1_INDEX)
    FAN1_INDEX == fanTempSensorIndex;
#else
    false;
#endif
}


bool constexpr isFan2TempSensorIndex(uint8_t fanTempSensorIndex)
{
    return
#if defined(FAN2_INDEX)
    FAN2_INDEX == fanTempSensorIndex;
#else
    false;
#endif
}


bool constexpr isFan3TempSensorIndex(uint8_t fanTempSensorIndex)
{
    return
#if defined(FAN3_INDEX)
    FAN3_INDEX == fanTempSensorIndex;
#else
    false;
#endif
}


bool constexpr isFan4TempSensorIndex(uint8_t fanTempSensorIndex)
{
    return
#if defined(FAN4_INDEX)
    FAN4_INDEX == fanTempSensorIndex;
#else
    false;
#endif
}


// clang-format off
#define isFanNDefinedAndHasIndexGenerator(x)                        \
bool constexpr isFan##x##DefinedAndHasIndex(uint8_t  fanIndex) \
{                                                                   \
    return isFan##x##Defined && isFan##x##Index(fanIndex);          \
}

isFanNDefinedAndHasIndexGenerator(0)
isFanNDefinedAndHasIndexGenerator(1)
isFanNDefinedAndHasIndexGenerator(2)
isFanNDefinedAndHasIndexGenerator(3)
isFanNDefinedAndHasIndexGenerator(4)

bool constexpr isAnyFanWithIndexDefined(const uint8_t fanIndex)
{
    return isFan0DefinedAndHasIndex(fanIndex) ||
           isFan1DefinedAndHasIndex(fanIndex) ||
           isFan2DefinedAndHasIndex(fanIndex) ||
           isFan3DefinedAndHasIndex(fanIndex) ||
           isFan4DefinedAndHasIndex(fanIndex);
}

#define isFanNDefinedAndHasTempSensorIndexGenerator(x)                           \
bool constexpr isFan##x##DefinedAndHasTempSensorIndex(uint8_t  sensorIndex) \
{                                                                                \
    return isFan##x##Defined && isFan##x##TempSensorIndex(sensorIndex);          \
}

isFanNDefinedAndHasTempSensorIndexGenerator(0)
isFanNDefinedAndHasTempSensorIndexGenerator(1)
isFanNDefinedAndHasTempSensorIndexGenerator(2)
isFanNDefinedAndHasTempSensorIndexGenerator(3)
isFanNDefinedAndHasTempSensorIndexGenerator(4)

bool constexpr isAnyFanWithIndexTempsensorDefined(const uint8_t  tempSensorIndex)
{
    return isFan1DefinedAndHasTempSensorIndex(tempSensorIndex) ||
           isFan1DefinedAndHasTempSensorIndex(tempSensorIndex) ||
           isFan1DefinedAndHasTempSensorIndex(tempSensorIndex) ||
           isFan1DefinedAndHasTempSensorIndex(tempSensorIndex) ||
           isFan1DefinedAndHasTempSensorIndex(tempSensorIndex);
}
// clang-format on

uint8_t constexpr getDefinedFansCount()
{
    uint8_t count{ 0 };
    count = (isFan0Defined) ? count + 1 : count;
    count = (isFan1Defined) ? count + 1 : count;
    count = (isFan2Defined) ? count + 1 : count;
    count = (isFan3Defined) ? count + 1 : count;
    count = (isFan4Defined) ? count + 1 : count;
    return count;
}

const uint8_t definedFanIndices[getDefinedFansCount()]
{
#if defined(FAN0)
    FAN0_INDEX,
#endif
#if defined(FAN1)
    FAN1_INDEX,
#endif
#if defined(FAN2)
    FAN2_INDEX,
#endif
#if defined(FAN3)
    FAN3_INDEX,
#endif
#if defined(FAN4)
    FAN4_INDEX,
#endif
};


uint8_t constexpr getDefinedTempSensorsCount()
{
    using Address = uint8_t[8];
    Address addresses[] TEMP_SENSORS_ADDRESS;
    return sizeof(addresses) / sizeof(Address);
}
