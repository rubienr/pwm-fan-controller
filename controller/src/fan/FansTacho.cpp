#include "FansTacho.h"
#include "configuration.h"
#include <Arduino.h>
#include <driver/gpio.h>
#include <driver/pcnt.h>


bool FanTachoSpecs::hasAlert() const { return currentRpm < alertBelowRpm || currentRpm > alertAboveRpm; }


bool FansTacho::begin()
{
    bool success{ true };

#if defined(FAN0)
    fans[FAN0_INDEX].alertBelowRpm = FAN0_ALERT_BELOW_RPM;
    fans[FAN0_INDEX].alertAboveRpm = FAN0_ALERT_ABOVE_RPM;
    fans[FAN0_INDEX].counterUnit = FAN0_TACHO_COUNTER_UNIT;
    fans[FAN0_INDEX].counterChannel = FAN0_TACHO_COUNTER_CHANNEL;
    fans[FAN0_INDEX].counterGpioNum = FAN0_TACHO_GPIO_NUM;
    if(!setupCounterUnit(FAN0_INDEX)) success = false;
#endif
#if defined(FAN1)
    fans[FAN1_INDEX].alertBelowRpm = FAN1_ALERT_BELOW_RPM;
    fans[FAN1_INDEX].alertAboveRpm = FAN1_ALERT_ABOVE_RPM;
    fans[FAN1_INDEX].counterUnit = FAN1_TACHO_COUNTER_UNIT;
    fans[FAN1_INDEX].counterChannel = FAN1_TACHO_COUNTER_CHANNEL;
    fans[FAN1_INDEX].counterGpioNum = FAN1_TACHO_GPIO_NUM;
    if(!setupCounterUnit(FAN1_INDEX)) success = false;
#endif
#if defined(FAN2)
    fans[FAN2_INDEX].alertBelowRpm = FAN2_ALERT_BELOW_RPM;
    fans[FAN2_INDEX].alertAboveRpm = FAN2_ALERT_ABOVE_RPM;
    fans[FAN2_INDEX].counterUnit = FAN2_TACHO_COUNTER_UNIT;
    fans[FAN2_INDEX].counterChannel = FAN2_TACHO_COUNTER_CHANNEL;
    fans[FAN2_INDEX].counterGpioNum = FAN2_TACHO_GPIO_NUM;
    if(!setupCounterUnit(FAN2_INDEX)) success = false;
#endif
#if defined(FAN3)
    fans[FAN3_INDEX].alertBelowRpm = FAN3_ALERT_BELOW_RPM;
    fans[FAN3_INDEX].alertAboveRpm = FAN3_ALERT_ABOVE_RPM;
    fans[FAN3_INDEX].counterUnit = FAN3_TACHO_COUNTER_UNIT;
    fans[FAN3_INDEX].counterChannel = FAN3_TACHO_COUNTER_CHANNEL;
    fans[FAN3_INDEX].counterGpioNum = FAN3_TACHO_GPIO_NUM;
    if(!setupCounterUnit(FAN3_INDEX)) success = false;
#endif
#if defined(FAN4)
    fans[FAN4_INDEX].alertBelowRpm = FAN4_ALERT_BELOW_RPM;
    fans[FAN4_INDEX].alertAboveRpm = FAN4_ALERT_ABOVE_RPM;
    fans[FAN4_INDEX].counterUnit = FAN4_TACHO_COUNTER_UNIT;
    fans[FAN4_INDEX].counterChannel = FAN4_TACHO_COUNTER_CHANNEL;
    fans[FAN4_INDEX].counterGpioNum = FAN4_TACHO_GPIO_NUM;
    if(!setupCounterUnit(FAN4_INDEX)) success = false;
#endif

    return success;
}


bool FansTacho::setupCounterUnit(uint8_t fanIndex)
{
    pcnt_config_t configuration{};
    configuration.ctrl_gpio_num = -1;
    configuration.pos_mode = pcnt_channel_edge_action_t::PCNT_CHANNEL_EDGE_ACTION_INCREASE;
    configuration.neg_mode = pcnt_channel_edge_action_t::PCNT_CHANNEL_EDGE_ACTION_HOLD;
    configuration.counter_l_lim = 0;
    configuration.counter_h_lim = 10000;

    const FanTachoSpecs &info{ fans[fanIndex] };
    pinMode(info.counterGpioNum, INPUT_PULLDOWN);
    configuration.pulse_gpio_num = info.counterGpioNum;
    configuration.unit = info.counterUnit;
    configuration.channel = info.counterChannel;
    return (ESP_OK == pcnt_unit_config(&configuration));
}


bool FansTacho::processEvery1000Ms()
{
    bool success{ true };

#if defined(FAN0)
    if(!takeFromCounterUnit(FAN0_INDEX)) success = false;
#endif
#if defined(FAN1)
    if(!takeFromCounterUnit(FAN1_INDEX)) success = false;
#endif
#if defined(FAN2)
    if(!takeFromCounterUnit(FAN2_INDEX)) success = false;
#endif
#if defined(FAN3)
    if(!takeFromCounterUnit(FAN3_INDEX)) success = false;
#endif
#if defined(FAN4)
    if(!takeFromCounterUnit(FAN4_INDEX)) success = false;
#endif

    return success;
}


bool FansTacho::takeFromCounterUnit(uint8_t fanIndex)
{
    FanTachoSpecs &info{ fans[fanIndex] };
    esp_err_t result{ pcnt_get_counter_value(info.counterUnit, &info.currentRpm) };
    info.currentRpm *= 30; // rpm = (impulses / 2) * 60
    pcnt_counter_clear(info.counterUnit);
    info.hasError = ESP_OK != result;
    return info.hasError;
}


const FanTachoSpecs &FansTacho::getSpecs(uint8_t fanIndex) const { return fans[fanIndex]; }


FanTachoSpecs &FansTacho::getSpecs(uint8_t fanIndex) { return fans[fanIndex]; }
