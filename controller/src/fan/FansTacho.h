#pragma once
#include <driver/pcnt.h>
#include <inttypes.h>


struct FansTacho
{
    bool begin();
    bool processEvery1000Ms();
    int16_t getRpm(uint8_t index);

protected:
    static bool setupCounterUnit(pcnt_unit_t unit, pcnt_channel_t unitChannel, uint8_t gpioNum);
    bool takeFromCounterUnit(pcnt_unit_t unit, uint8_t index);

    int16_t rpmX2[7]{ 0 };
};
