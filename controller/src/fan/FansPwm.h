#pragma once
#include <inttypes.h>


struct FanSpecs
{
    uint32_t minDuty{ 0 };
    uint32_t maxDuty{ 0 };
    uint32_t defaultDuty{ 0 };
    uint32_t errorDuty{ 0 };
    uint8_t pwmChannel{ 0 };
};


struct FansPwm
{
    bool begin();
    void setPwm(uint8_t index, uint32_t duty);

protected:
    bool configureFanPwm(uint8_t index,
                         uint8_t pwmChannel,
                         uint8_t resolutionBits,
                         uint32_t pwmFrequency,
                         uint32_t defaultDuty,
                         uint32_t errorDuty,
                         uint32_t minDuty,
                         uint32_t maxDuty,
                         uint8_t gpioNum);

    FanSpecs fans[7]{};
};
