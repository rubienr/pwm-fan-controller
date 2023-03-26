#pragma once
#include <cinttypes>


struct FanPwmSpec
{
    [[nodiscard]] bool hasAlert() const; // true if PWM is not within alertBelowDuty and alertAboveDuty

    uint32_t minDuty{ 0 };
    uint32_t maxDuty{ 0 };
    uint32_t defaultDuty{ 0 };
    uint32_t errorDuty{ 0 };
    uint32_t alertBelowDuty{ 0 };
    uint32_t alertAboveDuty{ 0 };
    uint8_t resolutionBids{ 0 };
    uint32_t frequencyHz{ 0 };
    uint8_t pwmChannel{ 0 };
    uint8_t pwmGpioNum{ 0 };
    uint32_t currentDuty{ 0 }; // w.r.t. FANx_PWM_MAX_DUTY
};


struct FansPwm
{
    bool begin();
    uint32_t setPwm(uint8_t fanIndex, uint32_t duty);
    uint32_t setDefaultPwm(uint8_t fanIndex);
    uint32_t setErrorPwm(uint8_t fanIndex);
    [[nodiscard]] const FanPwmSpec &getSpec(uint8_t fanIndex) const;
    [[nodiscard]] FanPwmSpec &getSpec(uint8_t fanIndex);


protected:
    bool configureFanPwm(uint8_t fanIndex);

    FanPwmSpec fans[5]{};
};
