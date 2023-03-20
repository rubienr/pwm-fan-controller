#include "FanSpeedInterpolator.h"
#include <cmath>


void FanSpeedInterpolator::interpolatePowerFromTemperature(const float &temperatureCelsius)
{
    auto reportedTemperature = static_cast<int16_t>(lroundf(temperatureCelsius * 10));

    if(reportedTemperature <= fanCurve[0].tempCentiCelsius) // min power
        interpolatedPower = fanCurve[0].power;
    else if(reportedTemperature <= fanCurve[1].tempCentiCelsius)
        interpolatedPower = interpolateSegment(0, 1, reportedTemperature);
    else if(reportedTemperature <= fanCurve[2].tempCentiCelsius)
        interpolatedPower = interpolateSegment(1, 2, reportedTemperature);
    else if(reportedTemperature <= fanCurve[3].tempCentiCelsius)
        interpolatedPower = interpolateSegment(2, 3, reportedTemperature);
    else // if (reportedTemperature > fanCurve[3].tempCentiCelsius) // max power
        interpolatedPower = fanCurve[3].power;
}


uint8_t FanSpeedInterpolator::interpolateSegment(uint8_t p0, uint8_t p1, const float &tempCentiCelsius) const
{
    float k, x, d;
    computeCoefficients(p0, p1, tempCentiCelsius, k, x, d);
    return static_cast<uint8_t>(roundf(k * x + d));
}


void FanSpeedInterpolator::computeCoefficients(uint8_t p0, uint8_t p1, const float &tempCentiCelsius, float &k, float &x, float &d) const
{
    const float dy{ static_cast<float>(fanCurve[p1].power - fanCurve[p0].power) };
    const float dx{ static_cast<float>(fanCurve[p1].tempCentiCelsius - fanCurve[p0].tempCentiCelsius) };

    k = dy / dx;
    x = tempCentiCelsius - static_cast<float>(fanCurve[p0].tempCentiCelsius);
    d = fanCurve[0].power;
}


void FanSpeedInterpolator::setPowerCurvePoints(const uint8_t (&power)[4], const int16_t (&tempCentiCelsius)[4])
{
    for(uint8_t idx = 0; idx < 4; idx++)
    {
        fanCurve[idx].power = power[idx];
        fanCurve[idx].tempCentiCelsius = tempCentiCelsius[idx];
    }
}

uint8_t FanSpeedInterpolator::getInterpolatedPower() const { return interpolatedPower; }
