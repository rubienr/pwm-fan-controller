#include "FanSpeedInterpolator.h"


uint8_t FanSpeedInterpolator::interpolate(const float &temperatureCelsius)
{
    int16_t reportedTemperature = static_cast<int16_t>((temperatureCelsius + 0.05) * 10);

    if(reportedTemperature <= fanCurve[0].tempCentiCelsius) // min power
        return fanCurve[0].power;
    else if(reportedTemperature <= fanCurve[1].tempCentiCelsius) return interpolateSegment(0, 1, reportedTemperature);
    else if(reportedTemperature <= fanCurve[2].tempCentiCelsius) return interpolateSegment(1, 2, reportedTemperature);
    else if(reportedTemperature <= fanCurve[3].tempCentiCelsius) return interpolateSegment(2, 3, reportedTemperature);
    else // if (reportedTemperature > fanCurve[3].tempCentiCelsius) // max power
        return fanCurve[3].power;
}


uint8_t FanSpeedInterpolator::interpolateSegment(uint8_t p0, uint8_t p1, const float &tempCentiCelsius)
{
    float k, x, d;
    computeCoefficients(p0, p1, tempCentiCelsius, k, x, d);
    return static_cast<uint8_t>((k * x + d) + 0.5);
}


void FanSpeedInterpolator::computeCoefficients(uint8_t p0, uint8_t p1, const float &tempCentiCelsius, float &k, float &x, float &d)
{
    const float dy = fanCurve[p1].power - fanCurve[p0].power;
    const float dx = fanCurve[p1].tempCentiCelsius - fanCurve[p0].tempCentiCelsius;

    k = dy / dx;
    x = tempCentiCelsius - fanCurve[p0].tempCentiCelsius;
    d = fanCurve[0].power;
}
