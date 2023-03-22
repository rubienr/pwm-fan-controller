#include "FansTacho.h"
#include "configuration.h"
#include <Arduino.h>
#include <driver/gpio.h>
#include <driver/pcnt.h>


bool FanTachoSpec::hasAlert() const { return currentRpm < alertBelowRpm || currentRpm > alertAboveRpm; }

#define initFanTachoSpec(n)                                                   \
    {                                                                         \
        fans[FAN##n##_INDEX].alertBelowRpm = FAN##n##_ALERT_BELOW_RPM;        \
        fans[FAN##n##_INDEX].alertAboveRpm = FAN##n##_ALERT_ABOVE_RPM;        \
        fans[FAN##n##_INDEX].counterUnit = FAN##n##_TACHO_COUNTER_UNIT;       \
        fans[FAN##n##_INDEX].counterChannel = FAN##n##_TACHO_COUNTER_CHANNEL; \
        fans[FAN##n##_INDEX].counterGpioNum = FAN##n##_TACHO_GPIO_NUM;        \
        if(!setupCounterUnit(FAN##n##_INDEX)) success = false;                \
    }

bool FansTacho::begin()
{
    bool success{ true };
    if(isFan0Defined) initFanTachoSpec(0);
    if(isFan1Defined) initFanTachoSpec(1);
    if(isFan2Defined) initFanTachoSpec(2);
    if(isFan3Defined) initFanTachoSpec(3);
    if(isFan4Defined) initFanTachoSpec(4);
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

    const FanTachoSpec &info{ fans[fanIndex] };
    pinMode(info.counterGpioNum, INPUT_PULLDOWN);
    configuration.pulse_gpio_num = info.counterGpioNum;
    configuration.unit = info.counterUnit;
    configuration.channel = info.counterChannel;
    return (ESP_OK == pcnt_unit_config(&configuration));
}


bool FansTacho::processEvery1000Ms()
{
    bool success{ true };
    for(auto fanIndex : definedFanIndices)
        success = !takeFromCounterUnit(fanIndex) ? false : success;
    return success;
}


bool FansTacho::takeFromCounterUnit(uint8_t fanIndex)
{
    FanTachoSpec &info{ fans[fanIndex] };
    esp_err_t result{ pcnt_get_counter_value(info.counterUnit, &info.currentRpm) };
    info.currentRpm *= 30; // rpm = (impulses / 2) * 60
    pcnt_counter_clear(info.counterUnit);
    info.hasError = ESP_OK != result;
    return info.hasError;
}


const FanTachoSpec &FansTacho::getSpecs(uint8_t fanIndex) const { return fans[fanIndex]; }


FanTachoSpec &FansTacho::getSpecs(uint8_t fanIndex) { return fans[fanIndex]; }
