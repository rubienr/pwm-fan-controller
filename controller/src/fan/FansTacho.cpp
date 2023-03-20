#include "FansTacho.h"
#include "configuration.h"
#include <Arduino.h>
#include <driver/gpio.h>
#include <soc/pcnt_struct.h>


bool FansTacho::begin()
{
    bool success{ true };

#if defined(FAN0)
    if(!setupCounterUnit(FAN0_TACHO_COUNTER_UNIT, FAN0_TACHO_COUNTER_CHANNEL, FAN0_TACHO_GPIO_NUM)) success = false;
#endif
#if defined(FAN1)
    if(!setupCounterUnit(FAN1_TACHO_COUNTER_UNIT, FAN1_TACHO_COUNTER_CHANNEL, FAN1_TACHO_GPIO_NUM)) success = false;
#endif
#if defined(FAN2)
    if(!setupCounterUnit(FAN2_TACHO_COUNTER_UNIT, FAN2_TACHO_COUNTER_CHANNEL, FAN2_TACHO_GPIO_NUM)) success = false;
#endif
#if defined(FAN3)
    if(!setupCounterUnit(FAN3_TACHO_COUNTER_UNIT, FAN3_TACHO_COUNTER_CHANNEL, FAN3_TACHO_GPIO_NUM)) success = false;
#endif
#if defined(FAN4)
    if(!setupCounterUnit(FAN4_TACHO_COUNTER_UNIT, FAN4_TACHO_COUNTER_CHANNEL, FAN4_TACHO_GPIO_NUM)) success = false;
#endif

    return success;
}


bool FansTacho::setupCounterUnit(pcnt_unit_t unit, pcnt_channel_t unitChannel, uint8_t gpioNum)
{
    pcnt_config_t configuration{};
    configuration.ctrl_gpio_num = -1;
    configuration.pos_mode = pcnt_channel_edge_action_t::PCNT_CHANNEL_EDGE_ACTION_INCREASE;
    configuration.neg_mode = pcnt_channel_edge_action_t::PCNT_CHANNEL_EDGE_ACTION_HOLD;
    configuration.counter_l_lim = 0;
    configuration.counter_h_lim = 10000;

    pinMode(gpioNum, INPUT_PULLDOWN);
    configuration.pulse_gpio_num = gpioNum;
    configuration.unit = unit;
    configuration.channel = unitChannel;
    return (ESP_OK == pcnt_unit_config(&configuration));
}


bool FansTacho::processEvery1000Ms()
{
    bool success{ true };

#if defined(FAN0)
    if(!takeFromCounterUnit(FAN0_TACHO_COUNTER_UNIT, FAN0_INDEX)) success = false;
#endif
#if defined(FAN1)
    if(!takeFromCounterUnit(FAN1_TACHO_COUNTER_UNIT, FAN1_INDEX)) success = false;
#endif
#if defined(FAN2)
    if(!takeFromCounterUnit(FAN2_TACHO_COUNTER_UNIT, FAN2_INDEX)) success = false;
#endif
#if defined(FAN3)
    if(!takeFromCounterUnit(FAN3_TACHO_COUNTER_UNIT, FAN3_INDEX)) success = false;
#endif
#if defined(FAN4)
    if(!takeFromCounterUnit(FAN4_TACHO_COUNTER_UNIT, FAN4_INDEX)) success = false;
#endif

    return success;
}


bool FansTacho::takeFromCounterUnit(pcnt_unit_t unit, uint8_t index)
{
    esp_err_t result{ pcnt_get_counter_value(unit, &rpmX2[index]) };
    pcnt_counter_clear(unit);
    return ESP_OK == result;
}


int16_t FansTacho::getRpm(uint8_t index)
{
    return static_cast<int16_t>(rpmX2[index] * 30); // (impulses / 2) * 60
}
