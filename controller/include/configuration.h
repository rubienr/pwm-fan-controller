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

#define FAN0                          // comment to disable feature
#if defined(FAN0)
    #define FAN0_TACHO_GPIO_NUM                36                             // tacho input pin
    #define FAN0_TACHO_COUNTER_UNIT            pcnt_unit_t::PCNT_UNIT_0       // signal counter unit
    #define FAN0_TACHO_COUNTER_CHANNEL         pcnt_channel_t::PCNT_CHANNEL_0 // signal counter unit-channel

    #define FAN0_PWM_GPIO_NUM                  25    // PWM output pin
    #define FAN0_PWM_CHANNEL                   0     // PWM channel
    #define FAN0_PWM_FREQUENCY_HZ              25000 // recommended 25kHz
    #define FAN0_PWM_RESOLUTION_BITS           8     // recommended 8bit
    #define FAN0_PWM_MIN_DUTY                  0     // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN0_PWM_MAX_DUTY                  255   // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN0_PWM_DEFAULT_DUTY              100   // duty on startup w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN0_PWM_ERROR_DUTY                255   // duty on error w.r.t. FANx_PWM_RESOLUTION_BITS

    #define FAN0_TEMP_SENSOR_INDEX             0 // temperature sensor this fan depends on
    // clang-format off
    #define FAN0_CURVE_POWER         {50,  50, 100, 100} // target power, 0-255
    #define FAN0_CURVE_DECI_CELSIUS { 0, 250, 700, 800} // input temperature; i.e. 123 corresponds to 12,34°C
// clang-format on

    #define FAN0_ALERT_BELOW_PWM               50 // trigger alert below PWM threshold w.r.t. FANx_PWM_MIN_DUTY, FANx_PWM_MAX_DUTY
    #define FAN0_ALERT_ABOVE_PWM               50 // trigger alert above PWM threshold w.r.t. FANx_PWM_MIN_DUTY, FANx_PWM_MAX_DUTY
    #define FAN0_ALERT_BELOW_RPM               0  // trigger alert below RPM threshold
    #define FAN0_ALERT_ABOVE_RPM               1500 // trigger alert above RPM threshold
    #define FAN0_ALERT_BELOW_TEMP_DECI_CELSIUS 0    // trigger alert below temperature threshold
    #define FAN0_ALERT_ABOVE_TEMP_DECI_CELSIUS 600  // trigger alert above temperature threshold

#endif

#define FAN1 // comment to disable feature
#if defined(FAN1)
    #define FAN1_TACHO_GPIO_NUM                39                             // tacho input pin
    #define FAN1_TACHO_COUNTER_UNIT            pcnt_unit_t::PCNT_UNIT_1       // signal counter unit
    #define FAN1_TACHO_COUNTER_CHANNEL         pcnt_channel_t::PCNT_CHANNEL_0 // signal counter unit-channel

    #define FAN1_PWM_GPIO_NUM                  16    // PWM output pin
    #define FAN1_PWM_CHANNEL                   1     // PWM channel
    #define FAN1_PWM_FREQUENCY_HZ              25000 // recommended 25kHz
    #define FAN1_PWM_RESOLUTION_BITS           8     // recommended 8bit
    #define FAN1_PWM_MIN_DUTY                  0     // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN1_PWM_MAX_DUTY                  255   // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN1_PWM_DEFAULT_DUTY              100   // duty on startup w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN1_PWM_ERROR_DUTY                255   // duty on error w.r.t. FANx_PWM_RESOLUTION_BITS

    #define FAN1_TEMP_SENSOR_INDEX             1 // temperature sensor this fan depends on

    // clang-format off
    #define FAN1_CURVE_POWER         {50,  50, 100, 100} // target power, 0-255
    #define FAN1_CURVE_DECI_CELSIUS { 0, 250, 700, 800} // input temperature; i.e. 123 corresponds to 12,34°C
// clang-format on

    #define FAN1_ALERT_BELOW_PWM               1 // trigger alert below PWM threshold w.r.t. FANx_PWM_MIN_DUTY, FANx_PWM_MAX_DUTY
    #define FAN1_ALERT_ABOVE_PWM               254 // trigger alert above PWM threshold w.r.t. FANx_PWM_MIN_DUTY, FANx_PWM_MAX_DUTY
    #define FAN1_ALERT_BELOW_RPM               0   // trigger alert below RPM threshold
    #define FAN1_ALERT_ABOVE_RPM               1500 // trigger alert above RPM threshold
    #define FAN1_ALERT_BELOW_TEMP_DECI_CELSIUS 0    // trigger alert below temperature threshold
    #define FAN1_ALERT_ABOVE_TEMP_DECI_CELSIUS 600  // trigger alert above temperature threshold
#endif

#define FAN2 // comment to disable feature
#if defined(FAN2)
    #define FAN2_TACHO_GPIO_NUM                0                              // tacho input pin
    #define FAN2_TACHO_COUNTER_UNIT            pcnt_unit_t::PCNT_UNIT_2       // signal counter unit
    #define FAN2_TACHO_COUNTER_CHANNEL         pcnt_channel_t::PCNT_CHANNEL_0 // signal counter unit-channel

    #define FAN2_PWM_GPIO_NUM                  2     // PWM output pin
    #define FAN2_PWM_CHANNEL                   2     // PWM channel
    #define FAN2_PWM_FREQUENCY_HZ              25000 // recommended 25kHz
    #define FAN2_PWM_RESOLUTION_BITS           8     // recommended 8bit
    #define FAN2_PWM_MIN_DUTY                  0     // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN2_PWM_MAX_DUTY                  255   // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN2_PWM_DEFAULT_DUTY              100   // duty on startup w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN2_PWM_ERROR_DUTY                255   // duty on error w.r.t. FANx_PWM_RESOLUTION_BITS

    #define FAN2_TEMP_SENSOR_INDEX             2 // temperature sensor this fan depends on

    // clang-format off
    #define FAN2_CURVE_POWER         {50,  50, 100, 100} // target power, 0-255
    #define FAN2_CURVE_DECI_CELSIUS { 0, 250, 700, 800} // input temperature; i.e. 123 corresponds to 12,34°C
// clang-format on

    #define FAN2_ALERT_BELOW_PWM               1 // trigger alert below PWM threshold w.r.t. FANx_PWM_MIN_DUTY, FANx_PWM_MAX_DUTY
    #define FAN2_ALERT_ABOVE_PWM               254 // trigger alert above PWM threshold w.r.t. FANx_PWM_MIN_DUTY, FANx_PWM_MAX_DUTY
    #define FAN2_ALERT_BELOW_RPM               0   // trigger alert below RPM threshold
    #define FAN2_ALERT_ABOVE_RPM               1500 // trigger alert above RPM threshold
    #define FAN2_ALERT_BELOW_TEMP_DECI_CELSIUS 0    // trigger alert below temperature threshold
    #define FAN2_ALERT_ABOVE_TEMP_DECI_CELSIUS 600  // trigger alert above temperature threshold
#endif

#define FAN3 // comment to disable feature
#if defined(FAN3)
    #define FAN3_TACHO_GPIO_NUM                14                             // tacho input pin
    #define FAN3_TACHO_COUNTER_UNIT            pcnt_unit_t::PCNT_UNIT_3       // signal counter unit
    #define FAN3_TACHO_COUNTER_CHANNEL         pcnt_channel_t::PCNT_CHANNEL_0 // signal counter unit-channel

    #define FAN3_PWM_GPIO_NUM                  12    // PWM output pin
    #define FAN3_PWM_CHANNEL                   3     // PWM channel
    #define FAN3_PWM_FREQUENCY_HZ              25000 // recommended 25kHz
    #define FAN3_PWM_RESOLUTION_BITS           8     // recommended 8bit
    #define FAN3_PWM_MIN_DUTY                  0     // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN3_PWM_MAX_DUTY                  255   // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN3_PWM_DEFAULT_DUTY              100   // duty on startup w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN3_PWM_ERROR_DUTY                255   // duty on error w.r.t. FANx_PWM_RESOLUTION_BITS

    #define FAN3_TEMP_SENSOR_INDEX             3 // temperature sensor this fan depends on

    // clang-format off
    #define FAN3_CURVE_POWER         {50,  50, 100, 100} // target power, 0-255
    #define FAN3_CURVE_DECI_CELSIUS { 0, 250, 700, 800} // input temperature; i.e. 123 corresponds to 12,34°C
// clang-format on

    #define FAN3_ALERT_BELOW_PWM               1 // trigger alert below PWM threshold w.r.t. FANx_PWM_MIN_DUTY, FANx_PWM_MAX_DUTY
    #define FAN3_ALERT_ABOVE_PWM               254 // trigger alert above PWM threshold w.r.t. FANx_PWM_MIN_DUTY, FANx_PWM_MAX_DUTY
    #define FAN3_ALERT_BELOW_RPM               0   // trigger alert below RPM threshold
    #define FAN3_ALERT_ABOVE_RPM               1500 // trigger alert above RPM threshold
    #define FAN3_ALERT_BELOW_TEMP_DECI_CELSIUS 0    // trigger alert below temperature threshold
    #define FAN3_ALERT_ABOVE_TEMP_DECI_CELSIUS 600  // trigger alert above temperature threshold
#endif

#define FAN4 // comment to disable feature
#if defined(FAN4)
    #define FAN4_TACHO_GPIO_NUM                13                             // tacho input pin
    #define FAN4_TACHO_COUNTER_UNIT            pcnt_unit_t::PCNT_UNIT_4       // signal counter unit
    #define FAN4_TACHO_COUNTER_CHANNEL         pcnt_channel_t::PCNT_CHANNEL_0 // signal counter unit-channel

    #define FAN4_PWM_GPIO_NUM                  15    // PWM output pin
    #define FAN4_PWM_CHANNEL                   4     // PWM channel
    #define FAN4_PWM_FREQUENCY_HZ              25000 // recommended 25kHz
    #define FAN4_PWM_RESOLUTION_BITS           8     // recommended 8bit
    #define FAN4_PWM_MIN_DUTY                  0     // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN4_PWM_MAX_DUTY                  255   // duty w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN4_PWM_DEFAULT_DUTY              100   // duty on startup w.r.t. FANx_PWM_RESOLUTION_BITS
    #define FAN4_PWM_ERROR_DUTY                255   // duty on error w.r.t. FANx_PWM_RESOLUTION_BITS

    #define FAN4_TEMP_SENSOR_INDEX             4 // temperature sensor this fan depends on

    // clang-format off
    #define FAN4_CURVE_POWER         {50,  50, 100, 100} // target power, 0-255
    #define FAN4_CURVE_DECI_CELSIUS { 0, 250, 700, 800} // input temperature; i.e. 123 corresponds to 12,34°C
// clang-format on

    #define FAN4_ALERT_BELOW_PWM               1 // trigger alert below PWM threshold w.r.t. FANx_PWM_MIN_DUTY, FANx_PWM_MAX_DUTY
    #define FAN4_ALERT_ABOVE_PWM               254 // trigger alert above PWM threshold w.r.t. FANx_PWM_MIN_DUTY, FANx_PWM_MAX_DUTY
    #define FAN4_ALERT_BELOW_RPM               0   // trigger alert below RPM threshold
    #define FAN4_ALERT_ABOVE_RPM               1500 // trigger alert above RPM threshold
    #define FAN4_ALERT_BELOW_TEMP_DECI_CELSIUS 0    // trigger alert below temperature threshold
    #define FAN4_ALERT_ABOVE_TEMP_DECI_CELSIUS 600  // trigger alert above temperature threshold

#endif

// ----- section temperature sensors (DS18xxx)

#define TEMPERATURE_SENSORS_ONE_WIRE_GPIO_NUM 26 // one wire pin
#define TEMPERATURE_SENSORS_PRECISION_BITS    10 // 9-12 bits; conversion time: 9-bit about 94ms, 12-bit about 750ms

// Define up to 5 sensors that shall match one or multiple fan(s) by index FANx_TEMP_SENSOR_INDEX.
// clang-format off
#define TEMPERATURE_SENSOR_0_ADDRESS { 0x28, 0xFF, 0x64, 0x02, 0x19, 0x86, 0x14, 0xFC }
#define TEMPERATURE_SENSOR_1_ADDRESS { 0x28, 0xD9, 0xEC, 0x09, 0x06, 0x00, 0x00, 0xF5 }
#define TEMPERATURE_SENSOR_2_ADDRESS { 0x28, 0xFF, 0x64, 0x02, 0x19, 0x8B, 0xD0, 0xDE }
#define TEMPERATURE_SENSOR_3_ADDRESS { 0x28, 0x64, 0x41, 0x0A, 0x06, 0x00, 0x00, 0xEF }
#define TEMPERATURE_SENSOR_4_ADDRESS { 0x28, 0x05, 0x52, 0x08, 0x06, 0x00, 0x00, 0x2E }
// clang-format on

// ----- section logging

#define SERIAL_AUTOREPORT                     // enable auto reporting to serial console
#if defined(SERIAL_AUTOREPORT)
    #define SERIAL_AUTOREPORT_EVERY_SECONDS 0 // 0=disabled, reports the fan status information every N seconds
#endif

// ----- section advanced configuration

#include "configuration_advanced.h"
#include "sanity_checks.h"
