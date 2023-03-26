#include "FansPwm.h"
#include "configuration.h"
#include <Arduino.h>


bool FanPwmSpec::hasAlert() const { return currentDuty < alertBelowDuty || currentDuty > alertAboveDuty; }

#define initFanPwmSpec(n)                                           \
    {                                                               \
        fans[FAN##n].minDuty = FAN##n##_PWM_MIN_DUTY;               \
        fans[FAN##n].maxDuty = FAN##n##_PWM_MAX_DUTY;               \
        fans[FAN##n].defaultDuty = FAN##n##_PWM_DEFAULT_DUTY;       \
        fans[FAN##n].errorDuty = FAN##n##_PWM_ERROR_DUTY;           \
        fans[FAN##n].alertBelowDuty = FAN##n##_ALERT_BELOW_PWM;     \
        fans[FAN##n].alertAboveDuty = FAN##n##_ALERT_ABOVE_PWM;     \
        fans[FAN##n].resolutionBids = FAN##n##_PWM_RESOLUTION_BITS; \
        fans[FAN##n].frequencyHz = FAN##n##_PWM_FREQUENCY_HZ;       \
        fans[FAN##n].pwmChannel = FAN##n##_PWM_CHANNEL;             \
        fans[FAN##n].pwmGpioNum = FAN##n##_PWM_GPIO_NUM;            \
        if(!configureFanPwm(FAN##n)) success = false;               \
    }

bool FansPwm::begin()
{
    bool success{ true };
    if(isFan0Defined) initFanPwmSpec(0);
    if(isFan1Defined) initFanPwmSpec(1);
    if(isFan2Defined) initFanPwmSpec(2);
    if(isFan3Defined) initFanPwmSpec(3);
    if(isFan4Defined) initFanPwmSpec(4);
    return success;
}


bool FansPwm::configureFanPwm(uint8_t fanIndex)
{
    const FanPwmSpec &info{ fans[fanIndex] };
    uint32_t configuredFrequency{ ledcSetup(info.pwmChannel, info.frequencyHz, info.resolutionBids) };
    ledcAttachPin(info.pwmGpioNum, info.pwmChannel);
    setPwm(fanIndex, info.defaultDuty);
    return configuredFrequency == info.frequencyHz;
}


uint32_t FansPwm::setPwm(uint8_t fanIndex, uint32_t duty)
{
    FanPwmSpec &info{ fans[fanIndex] };
    info.currentDuty = min(duty, info.maxDuty);
    info.currentDuty = max(info.currentDuty, info.minDuty);
    ledcWrite(info.pwmChannel, info.currentDuty);
    return info.currentDuty;
}


uint32_t FansPwm::setDefaultPwm(uint8_t fanIndex) { return setPwm(fanIndex, fans[fanIndex].defaultDuty); }


uint32_t FansPwm::setErrorPwm(uint8_t fanIndex) { return setPwm(fanIndex, fans[fanIndex].errorDuty); }


const FanPwmSpec &FansPwm::getSpecs(uint8_t fanIndex) const { return fans[fanIndex]; }

FanPwmSpec &FansPwm::getSpecs(uint8_t fanIndex) { return fans[fanIndex]; }
