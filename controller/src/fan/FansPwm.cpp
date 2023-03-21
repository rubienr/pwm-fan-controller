#include "FansPwm.h"
#include "configuration.h"
#include <Arduino.h>


bool FanPwmSpecs::hasAlert() const { return currentDuty < alertBelowDuty || currentDuty > alertAboveDuty; }


bool FansPwm::begin()
{
    bool success{ true };

#if defined(FAN0)
    fans[FAN0_INDEX].minDuty = FAN0_PWM_MIN_DUTY;
    fans[FAN0_INDEX].maxDuty = FAN0_PWM_MAX_DUTY;
    fans[FAN0_INDEX].defaultDuty = FAN0_PWM_DEFAULT_DUTY;
    fans[FAN0_INDEX].errorDuty = FAN0_PWM_ERROR_DUTY;
    fans[FAN0_INDEX].alertBelowDuty = FAN0_ALERT_BELOW_PWM;
    fans[FAN0_INDEX].alertAboveDuty = FAN0_ALERT_ABOVE_PWM;
    fans[FAN0_INDEX].resolutionBids = FAN0_PWM_RESOLUTION_BITS;
    fans[FAN0_INDEX].frequencyHz = FAN0_PWM_FREQUENCY_HZ;
    fans[FAN0_INDEX].pwmChannel = FAN0_PWM_CHANNEL;
    fans[FAN0_INDEX].pwmGpioNum = FAN0_PWM_GPIO_NUM;
    if(!configureFanPwm(FAN0_INDEX)) success = false;
#endif
#if defined(FAN1)
    fans[FAN1_INDEX].minDuty = FAN1_PWM_MIN_DUTY;
    fans[FAN1_INDEX].maxDuty = FAN1_PWM_MAX_DUTY;
    fans[FAN1_INDEX].defaultDuty = FAN1_PWM_DEFAULT_DUTY;
    fans[FAN1_INDEX].errorDuty = FAN1_PWM_ERROR_DUTY;
    fans[FAN1_INDEX].alertBelowDuty = FAN1_ALERT_BELOW_PWM;
    fans[FAN1_INDEX].alertAboveDuty = FAN1_ALERT_ABOVE_PWM;
    fans[FAN1_INDEX].resolutionBids = FAN1_PWM_RESOLUTION_BITS;
    fans[FAN1_INDEX].frequencyHz = FAN1_PWM_FREQUENCY_HZ;
    fans[FAN1_INDEX].pwmChannel = FAN1_PWM_CHANNEL;
    fans[FAN1_INDEX].pwmGpioNum = FAN1_PWM_GPIO_NUM;
    if(!configureFanPwm(FAN1_INDEX)) success = false;
#endif
#if defined(FAN2)
    fans[FAN2_INDEX].minDuty = FAN2_PWM_MIN_DUTY;
    fans[FAN2_INDEX].maxDuty = FAN2_PWM_MAX_DUTY;
    fans[FAN2_INDEX].defaultDuty = FAN2_PWM_DEFAULT_DUTY;
    fans[FAN2_INDEX].errorDuty = FAN2_PWM_ERROR_DUTY;
    fans[FAN2_INDEX].alertBelowDuty = FAN2_ALERT_BELOW_PWM;
    fans[FAN2_INDEX].alertAboveDuty = FAN2_ALERT_ABOVE_PWM;
    fans[FAN2_INDEX].resolutionBids = FAN2_PWM_RESOLUTION_BITS;
    fans[FAN2_INDEX].frequencyHz = FAN2_PWM_FREQUENCY_HZ;
    fans[FAN2_INDEX].pwmChannel = FAN2_PWM_CHANNEL;
    fans[FAN2_INDEX].pwmGpioNum = FAN2_PWM_GPIO_NUM;
    if(!configureFanPwm(FAN2_INDEX)) success = false;
#endif
#if defined(FAN3)
    fans[FAN3_INDEX].minDuty = FAN3_PWM_MIN_DUTY;
    fans[FAN3_INDEX].maxDuty = FAN3_PWM_MAX_DUTY;
    fans[FAN3_INDEX].defaultDuty = FAN3_PWM_DEFAULT_DUTY;
    fans[FAN3_INDEX].errorDuty = FAN3_PWM_ERROR_DUTY;
    fans[FAN3_INDEX].alertBelowDuty = FAN3_ALERT_BELOW_PWM;
    fans[FAN3_INDEX].alertAboveDuty = FAN3_ALERT_ABOVE_PWM;
    fans[FAN3_INDEX].resolutionBids = FAN3_PWM_RESOLUTION_BITS;
    fans[FAN3_INDEX].frequencyHz = FAN3_PWM_FREQUENCY_HZ;
    fans[FAN3_INDEX].pwmChannel = FAN3_PWM_CHANNEL;
    fans[FAN3_INDEX].pwmGpioNum = FAN3_PWM_GPIO_NUM;
    if(!configureFanPwm(FAN3_INDEX)) success = false;
#endif
#if defined(FAN4)
    fans[FAN4_INDEX].minDuty = FAN4_PWM_MIN_DUTY;
    fans[FAN4_INDEX].maxDuty = FAN4_PWM_MAX_DUTY;
    fans[FAN4_INDEX].defaultDuty = FAN4_PWM_DEFAULT_DUTY;
    fans[FAN4_INDEX].errorDuty = FAN4_PWM_ERROR_DUTY;
    fans[FAN4_INDEX].alertBelowDuty = FAN4_ALERT_BELOW_PWM;
    fans[FAN4_INDEX].alertAboveDuty = FAN4_ALERT_ABOVE_PWM;
    fans[FAN4_INDEX].resolutionBids = FAN4_PWM_RESOLUTION_BITS;
    fans[FAN4_INDEX].frequencyHz = FAN4_PWM_FREQUENCY_HZ;
    fans[FAN4_INDEX].pwmChannel = FAN4_PWM_CHANNEL;
    fans[FAN4_INDEX].pwmGpioNum = FAN4_PWM_GPIO_NUM;
    if(!configureFanPwm(FAN4_INDEX)) success = false;
#endif

    return success;
}


bool FansPwm::configureFanPwm(uint8_t fanIndex)
{
    const FanPwmSpecs &info{ fans[fanIndex] };
    uint32_t configuredFrequency{ ledcSetup(info.pwmChannel, info.frequencyHz, info.resolutionBids) };
    ledcAttachPin(info.pwmGpioNum, info.pwmChannel);
    setPwm(fanIndex, info.defaultDuty);
    return configuredFrequency == info.frequencyHz;
}


uint32_t FansPwm::setPwm(uint8_t fanIndex, uint32_t duty)
{
    FanPwmSpecs &info{ fans[fanIndex] };
    info.currentDuty = min(duty, info.maxDuty);
    info.currentDuty = max(info.currentDuty, info.minDuty);
    ledcWrite(info.pwmChannel, info.currentDuty);
    return info.currentDuty;
}


uint32_t FansPwm::setDefaultPwm(uint8_t fanIndex) { return setPwm(fanIndex, fans[fanIndex].defaultDuty); }


uint32_t FansPwm::setErrorPwm(uint8_t fanIndex) { return setPwm(fanIndex, fans[fanIndex].errorDuty); }


const FanPwmSpecs &FansPwm::getSpecs(uint8_t fanIndex) const { return fans[fanIndex]; }

FanPwmSpecs &FansPwm::getSpecs(uint8_t fanIndex) { return fans[fanIndex]; }
