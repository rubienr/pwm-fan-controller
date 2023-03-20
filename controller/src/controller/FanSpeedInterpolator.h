#include <inttypes.h>


struct CurvePoint
{
    uint8_t power;            // on a scale from 0 to 255
    int16_t tempCentiCelsius; // i.e. 12.34°C corresponds to 123
};


struct FanSpeedInterpolator
{
    uint8_t interpolatePowerFromTemperature(const float &temperatureCelsius) const;
    void setPowerCurvePoints(const uint8_t (&power)[4], const int16_t (&tempCentiCelsius)[4]);

protected:
    uint8_t interpolateSegment(uint8_t p0, uint8_t p1, const float &tempCentiCelsius) const;
    void computeCoefficients(uint8_t p0, uint8_t p1, const float &tempCentiCelsius, float &k, float &x, float &d) const;

    CurvePoint fanCurve[4];
};
