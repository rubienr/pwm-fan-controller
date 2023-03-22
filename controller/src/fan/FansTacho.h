#pragma once
#include <cinttypes>
#include <driver/pcnt.h>

struct FanTachoSpec
{
    [[nodiscard]] bool hasAlert() const; // true if RPM is not within alertBelowRpm and alertAboveRpm
    bool hasError{ false };              // true if tacho readout was erroneous
    uint32_t alertBelowRpm{ 0 };
    uint32_t alertAboveRpm{ 0 };
    pcnt_unit_t counterUnit{};
    pcnt_channel_t counterChannel{};
    uint8_t counterGpioNum{ 0 };
    int16_t currentRpm{ 0 }; // measured RPM in last time period of 1000ms
};

struct FansTacho
{
    bool begin();
    bool processEvery1000Ms();
    [[nodiscard]] const FanTachoSpec &getSpecs(uint8_t fanIndex) const;
    [[nodiscard]] FanTachoSpec &getSpecs(uint8_t fanIndex);

protected:
    bool setupCounterUnit(uint8_t fanIndex);
    bool takeFromCounterUnit(uint8_t fanIndex);

    FanTachoSpec fans[5]{};
};
