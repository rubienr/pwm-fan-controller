#include "FansPwm.h"
#include "configuration.h"
#include <Arduino.h>


bool FansPwm::begin()
{
    bool success{ true };

#if defined(FAN0)
    success = configureFanPwm(FAN0_INDEX, FAN0_PWM_CHANNEL, FAN0_PWM_RESOLUTION_BITS, FAN0_PWM_FREQUENCY_HZ, FAN0_PWM_DEFAULT_DUTY,
                              FAN0_PWM_ERROR_DUTY, FAN0_PWM_MIN_DUTY, FAN0_PWM_MAX_DUTY, FAN0_PWM_GPIO_NUM) ?
              success :
              false;
#endif
#if defined(FAN1)
    success = configureFanPwm(FAN1_INDEX, FAN1_PWM_CHANNEL, FAN1_PWM_RESOLUTION_BITS, FAN1_PWM_FREQUENCY_HZ, FAN1_PWM_DEFAULT_DUTY,
                              FAN1_PWM_ERROR_DUTY, FAN1_PWM_MIN_DUTY, FAN1_PWM_MAX_DUTY, FAN1_PWM_GPIO_NUM) ?
              success :
              false;
#endif
#if defined(FAN2)
    success = configureFanPwm(FAN2_INDEX, FAN2_PWM_CHANNEL, FAN2_PWM_RESOLUTION_BITS, FAN2_PWM_FREQUENCY_HZ, FAN2_PWM_DEFAULT_DUTY,
                              FAN2_PWM_ERROR_DUTY, FAN2_PWM_MIN_DUTY, FAN2_PWM_MAX_DUTY, FAN2_PWM_GPIO_NUM) ?
              success :
              false;
#endif
#if defined(FAN3)
    success = configureFanPwm(FAN3_INDEX, FAN3_PWM_CHANNEL, FAN3_PWM_RESOLUTION_BITS, FAN3_PWM_FREQUENCY_HZ, FAN3_PWM_DEFAULT_DUTY,
                              FAN3_PWM_ERROR_DUTY, FAN3_PWM_MIN_DUTY, FAN3_PWM_MAX_DUTY, FAN3_PWM_GPIO_NUM) ?
              success :
              false;
#endif
#if defined(FAN4)
    success = configureFanPwm(FAN4_INDEX, FAN4_PWM_CHANNEL, FAN4_PWM_RESOLUTION_BITS, FAN4_PWM_FREQUENCY_HZ, FAN4_PWM_DEFAULT_DUTY,
                              FAN4_PWM_ERROR_DUTY, FAN4_PWM_MIN_DUTY, FAN4_PWM_MAX_DUTY, FAN4_PWM_GPIO_NUM) ?
              success :
              false;
#endif

    return success;
}


bool FansPwm::configureFanPwm(uint8_t fanIndex,
                              uint8_t pwmChannel,
                              uint8_t resolutionBits,
                              uint32_t pwmFrequency,
                              uint32_t defaultDuty,
                              uint32_t errorDuty,
                              uint32_t minDuty,
                              uint32_t maxDuty,
                              uint8_t gpioNum)
{
    fans[fanIndex].minDuty = minDuty;
    fans[fanIndex].maxDuty = maxDuty;
    fans[fanIndex].defaultDuty = defaultDuty;
    fans[fanIndex].errorDuty = errorDuty;
    fans[fanIndex].pwmChannel = pwmChannel;

    uint32_t configuredFrequency{ ledcSetup(pwmChannel, pwmFrequency, resolutionBits) };
    ledcAttachPin(gpioNum, pwmChannel);
    setPwm(fanIndex, defaultDuty);
    return configuredFrequency == pwmFrequency;
}


void FansPwm::setPwm(uint8_t index, uint32_t duty)
{
    duty = min(duty, fans[index].maxDuty);
    duty = max(duty, fans[index].minDuty);
    ledcWrite(fans[index].pwmChannel, duty);
}
