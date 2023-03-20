#include "FansController.h"
#include <cmath>

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
    sensors.captureTemperature();
    hasTachoError = !tachos.processEvery1000Ms();
#if defined(FAN0)
    updateFanInfo(FAN0_INDEX, FAN0_TEMP_SENSOR_INDEX, FAN0_PWM_MAX_DUTY, FAN0_PWM_ERROR_DUTY, FAN0_ALERT_BELOW_PWM,
                  FAN0_ALERT_ABOVE_PWM, FAN0_ALERT_BELOW_RPM, FAN0_ALERT_ABOVE_RPM, FAN0_ALERT_BELOW_TEMP_C, FAN0_ALERT_ABOVE_TEMP_C);
#endif
#if defined(FAN1)
    updateFanInfo(FAN1_INDEX, FAN1_TEMP_SENSOR_INDEX, FAN1_PWM_MAX_DUTY, FAN1_PWM_ERROR_DUTY, FAN1_ALERT_BELOW_PWM,
                  FAN1_ALERT_ABOVE_PWM, FAN1_ALERT_BELOW_RPM, FAN1_ALERT_ABOVE_RPM, FAN1_ALERT_BELOW_TEMP_C, FAN1_ALERT_ABOVE_TEMP_C);
#endif
#if defined(FAN2)
    updateFanInfo(FAN2_INDEX, FAN2_TEMP_SENSOR_INDEX, FAN2_PWM_MAX_DUTY, FAN2_PWM_ERROR_DUTY, FAN2_ALERT_BELOW_PWM,
                  FAN2_ALERT_ABOVE_PWM, FAN2_ALERT_BELOW_RPM, FAN2_ALERT_ABOVE_RPM, FAN2_ALERT_BELOW_TEMP_C, FAN2_ALERT_ABOVE_TEMP_C);
#endif
#if defined(FAN3)
    updateFanInfo(FAN3_INDEX, FAN3_TEMP_SENSOR_INDEX, FAN3_PWM_MAX_DUTY, FAN3_PWM_ERROR_DUTY, FAN3_ALERT_BELOW_PWM,
                  FAN3_ALERT_ABOVE_PWM, FAN3_ALERT_BELOW_RPM, FAN3_ALERT_ABOVE_RPM, FAN3_ALERT_BELOW_TEMP_C, FAN3_ALERT_ABOVE_TEMP_C);
#endif
#if defined(FAN4)
    updateFanInfo(FAN4_INDEX, FAN4_TEMP_SENSOR_INDEX, FAN4_PWM_MAX_DUTY, FAN4_PWM_ERROR_DUTY, FAN4_ALERT_BELOW_PWM,
                  FAN4_ALERT_ABOVE_PWM, FAN4_ALERT_BELOW_RPM, FAN4_ALERT_ABOVE_RPM, FAN4_ALERT_BELOW_TEMP_C, FAN4_ALERT_ABOVE_TEMP_C);
#endif
}


void FansController::updateFanInfo(uint8_t fanIndex,
                                   uint8_t fanTempSensorIndex,
                                   uint16_t maxPwmDuty,
                                   uint16_t fanErrorPwm,
                                   uint16_t fanAlertBelowPwm,
                                   uint16_t fanAlertAbovePwm,
                                   uint16_t fanAlertBelowRpm,
                                   uint16_t fanAlertAboveRpm,
                                   float fanAlertBelowTempCelsius,
                                   float fanAlertAboveTempCelsius)
{
    fansInfo[fanIndex].hasTachoError = hasTachoError;
    fansInfo[fanIndex].rpm = tachos.getRpm(fanIndex);

    fansInfo[fanIndex].hasTempError = !sensors.readTemperatureCelsius(fanTempSensorIndex, fansInfo[fanIndex].tempCelsius);

    if(fansInfo[fanIndex].hasTachoError or fansInfo[fanIndex].hasTempError) { fansInfo[fanIndex].powerPwm = fanErrorPwm; }
    else
    {
        fansInfo[fanIndex].power = fansInfo[fanIndex].interpolator.interpolatePowerFromTemperature(fansInfo[fanIndex].tempCelsius);
        float powerRatio{ (static_cast<float>(fansInfo[fanIndex].power) / 255.0f) };
        fansInfo[fanIndex].powerPercent = powerRatio * 100.0f;
        auto newPowerPwm = static_cast<uint16_t>(roundf(powerRatio * static_cast<float>(maxPwmDuty)));

        if(fansInfo[fanIndex].powerPwm == newPowerPwm) fansInfo[fanIndex].trend = 0;
        else if(fansInfo[fanIndex].powerPwm > newPowerPwm) fansInfo[fanIndex].trend = -1;
        else fansInfo[fanIndex].trend = 1;

        fansInfo[fanIndex].powerPwm = newPowerPwm;
        pwms.setPwm(fanIndex, fansInfo[fanIndex].powerPwm);
    }
    fansInfo[fanIndex].hasPwmAlert = (fansInfo[fanIndex].powerPwm < fanAlertBelowPwm) || (fansInfo[fanIndex].powerPwm > fanAlertAbovePwm);
    fansInfo[fanIndex].hasRpmAlert = (fansInfo[fanIndex].rpm < fanAlertBelowRpm) || (fansInfo[fanIndex].rpm > fanAlertAboveRpm);
    fansInfo[fanIndex].hasTempAlert =
    (fansInfo[fanIndex].tempCelsius < fanAlertBelowTempCelsius) || (fansInfo[fanIndex].tempCelsius > fanAlertAboveTempCelsius);
}
