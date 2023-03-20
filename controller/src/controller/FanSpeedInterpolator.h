#include <inttypes.h>


struct CurvePoint
{
    uint8_t power;            // on a scale from 0 to 255
    int16_t tempCentiCelsius; // i.e. 12.34°C corresponds to 123
};


struct FanSpeedInterpolator
{
    uint8_t interpolate(const float &temperatureCelsius);
    void setCurvePoints(const uint8_t (&power)[4], const int16_t (&tempCentiCelsius)[4])
    {
        for(uint8_t idx = 0; idx < 4; idx++)
        {
            fanCurve[idx].power = power[idx];
            fanCurve[idx].tempCentiCelsius = tempCentiCelsius[idx];
        }
    }

protected:
    uint8_t interpolateSegment(uint8_t p0, uint8_t p1, const float &tempCentiCelsius);
    void computeCoefficients(uint8_t p0, uint8_t p1, const float &tempCentiCelsius, float &k, float &x, float &d);

    CurvePoint fanCurve[4];
};
