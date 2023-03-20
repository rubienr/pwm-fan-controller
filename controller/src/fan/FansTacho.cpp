#include "FansTacho.h"
#include "configuration.h"
#include <Arduino.h>
#include <driver/gpio.h>
#include <soc/pcnt_struct.h>


bool FansTacho::begin()
{
    bool success{ true };

#if defined(FAN0)
    success = (!setupCounterUnit(FAN0_TACHO_COUNTER_UNIT, FAN0_TACHO_COUNTER_CHANNEL, FAN0_TACHO_GPIO_NUM)) ? false : success;
#endif
#if defined(FAN1)
    success = (!setupCounterUnit(FAN1_TACHO_COUNTER_UNIT, FAN1_TACHO_COUNTER_CHANNEL, FAN1_TACHO_GPIO_NUM)) ? false : success;
#endif
#if defined(FAN2)
    success = (!setupCounterUnit(FAN2_TACHO_COUNTER_UNIT, FAN2_TACHO_COUNTER_CHANNEL, FAN2_TACHO_GPIO_NUM)) ? false : success;
#endif
#if defined(FAN3)
    success = (!setupCounterUnit(FAN3_TACHO_COUNTER_UNIT, FAN3_TACHO_COUNTER_CHANNEL, FAN3_TACHO_GPIO_NUM)) ? false : success;
#endif
#if defined(FAN4)
    success = (!setupCounterUnit(FAN4_TACHO_COUNTER_UNIT, FAN4_TACHO_COUNTER_CHANNEL, FAN4_TACHO_GPIO_NUM)) ? false : success;
#endif
#if defined(FAN5)
    success = (!setupCounterUnit(FAN5_TACHO_COUNTER_UNIT, FAN5_TACHO_COUNTER_CHANNEL, FAN5_TACHO_GPIO_NUM)) ? false : success;
#endif
#if defined(FAN6)
    success = (!setupCounterUnit(FAN6_TACHO_COUNTER_UNIT, FAN6_TACHO_COUNTER_CHANNEL, FAN6_TACHO_GPIO_NUM)) ? false : success;
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


bool FansTacho::processEverySecond()
{
    bool success{ true };

#if defined(FAN0)
    success = (!takeFromCounterUnit(FAN0_TACHO_COUNTER_UNIT, FAN0_INDEX)) ? false : success;
#endif
#if defined(FAN1)
    success = (!takeFromCounterUnit(FAN1_TACHO_COUNTER_UNIT, FAN1_INDEX)) ? false : success;
#endif
#if defined(FAN2)
    success = (!takeFromCounterUnit(FAN2_TACHO_COUNTER_UNIT, FAN2_INDEX)) ? false : success;
#endif
#if defined(FAN3)
    success = (!takeFromCounterUnit(FAN3_TACHO_COUNTER_UNIT, FAN3_INDEX)) ? false : success;
#endif
#if defined(FAN4)
    success = (!takeFromCounterUnit(FAN4_TACHO_COUNTER_UNIT, FAN4_INDEX)) ? false : success;
#endif
#if defined(FAN5)
    success = (!takeFromCounterUnit(FAN5_TACHO_COUNTER_UNIT, FAN5_INDEX)) ? false : success;
#endif
#if defined(FAN6)
    success = (!takeFromCounterUnit(FAN6_TACHO_COUNTER_UNIT, FAN6_INDEX)) ? false : success;
#endif

    return success;
}


bool FansTacho::takeFromCounterUnit(pcnt_unit_t unit, uint8_t index)
{

    // pcnt_counter_pause(unit);
    esp_err_t result{ pcnt_get_counter_value(unit, &rpm_x2[index]) };
    pcnt_counter_clear(unit);
    // pcnt_counter_resume(unit);
    return ESP_OK == result;
}


int16_t FansTacho::getRpm(uint8_t index)
{
    return rpm_x2[index] * 30; // (impulses / 2) * 60
}
