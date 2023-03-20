#include "FansController.h"


FansController::FansController(SensorsTemp &sensors, FansPwm &pwms, FansTacho &tachos) :
sensors(sensors), pwms(pwms), tachos(tachos)
{
#if defined(FAN0)
    fansInfo[FAN0_INDEX].interpolator.setPowerCurvePoints(FAN0_CURVE_POWER, FAN0_CURVE_CENTI_CELSIUS);
#endif
#if defined(FAN1)
    fansInfo[FAN1_INDEX].interpolator.setPowerCurvePoints(FAN1_CURVE_POWER, FAN1_CURVE_CENTI_CELSIUS);
#endif
#if defined(FAN2)
    fansInfo[FAN2_INDEX].interpolator.setPowerCurvePoints(FAN2_CURVE_POWER, FAN2_CURVE_CENTI_CELSIUS);
#endif
#if defined(FAN3)
    fansInfo[FAN3_INDEX].interpolator.setPowerCurvePoints(FAN3_CURVE_POWER, FAN3_CURVE_CENTI_CELSIUS);
#endif
#if defined(FAN4)
    fansInfo[FAN4_INDEX].interpolator.setPowerCurvePoints(FAN4_CURVE_POWER, FAN4_CURVE_CENTI_CELSIUS);
#endif
}


void FansController::process()
{
    sensors.captureTemperature(); // todo - ret error
    tachos.processEverySecond();  // todo - ret error
#if defined(FAN0)
    updateFanInfo(FAN0_INDEX, FAN0_TEMP_SENSOR_INDEX, FAN0_PWM_MAX_DUTY);
#endif
#if defined(FAN1)
    updateFanInfo(FAN1_INDEX, FAN1_TEMP_SENSOR_INDEX, FAN1_PWM_MAX_DUTY);
#endif
#if defined(FAN2)
    updateFanInfo(FAN2_INDEX, FAN2_TEMP_SENSOR_INDEX, FAN2_PWM_MAX_DUTY);
#endif
#if defined(FAN3)
    updateFanInfo(FAN3_INDEX, FAN3_TEMP_SENSOR_INDEX, FAN3_PWM_MAX_DUTY);
#endif
#if defined(FAN4)
    updateFanInfo(FAN4_INDEX, FAN4_TEMP_SENSOR_INDEX, FAN4_PWM_MAX_DUTY);
#endif
}


void FansController::updateFanInfo(uint8_t fanIndex, uint8_t fanTempSensorIndex, uint16_t /*maxPwmDuty*/)
{
    fansInfo[fanIndex].rpm = tachos.getRpm(fanIndex);
    sensors.readTemperatureCelsius(fanTempSensorIndex, fansInfo[fanIndex].tempCelsius);
    fansInfo[fanIndex].power = fansInfo[fanIndex].interpolator.interpolatePowerFromTemperature(fansInfo[fanIndex].tempCelsius);
    fansInfo[fanIndex].powerPwm = fansInfo[fanIndex].power; // (255 / fansInfo[fanIndex].power) * maxPwmDuty;
    pwms.setPwm(fanIndex, fansInfo[fanIndex].powerPwm);
}
