#include <cinttypes>
#include <cmath>


struct CurvePoint
{
    uint8_t power;            // on a scale from 0 to 255
    int16_t tempCentiCelsius; // i.e. 12.34°C corresponds to 123
};


template <uint8_t NumPoints = 4> struct FanSpeedInterpolator
{
    using FanCurveType = CurvePoint[NumPoints];
    static_assert(NumPoints > 1);

    bool setPowerCurvePoints(const uint8_t (&power)[NumPoints], const int16_t (&tempCentiCelsius)[NumPoints])
    {
        // assert: tempCentiCelsius[N] < tempCentiCelsius[N+1]
        for(uint8_t idx = 1; idx < NumPoints; idx++)
            if(tempCentiCelsius[idx - 1] >= tempCentiCelsius[idx]) return false;

        for(uint8_t idx = 0; idx < NumPoints; idx++)
        {
            fanCurve[idx].power = power[idx];
            fanCurve[idx].tempCentiCelsius = tempCentiCelsius[idx];
        }
        return true;
    }

    void interpolatePowerFromTemperature(const float &temperatureCelsius)
    {
        auto reportedTemperature = static_cast<int16_t>(lroundf(temperatureCelsius * 10));


        if(reportedTemperature <= fanCurve[0].tempCentiCelsius) // min power
            interpolatedPower = fanCurve[0].power;
        else if(reportedTemperature <= fanCurve[1].tempCentiCelsius)
            interpolatedPower = interpolateFromSegment(0, 1, reportedTemperature);
        else if(reportedTemperature <= fanCurve[2].tempCentiCelsius)
            interpolatedPower = interpolateFromSegment(1, 2, reportedTemperature);
        else if(reportedTemperature <= fanCurve[3].tempCentiCelsius)
            interpolatedPower = interpolateFromSegment(2, 3, reportedTemperature);
        else // if (reportedTemperature > fanCurve[3].tempCentiCelsius) // max power
            interpolatedPower = fanCurve[3].power;
    }


    [[nodiscard]] uint8_t getInterpolatedPower() const { return interpolatedPower; }


    [[nodiscard]] const FanCurveType &getCurve() const { return fanCurve; }


    [[nodiscard]] FanCurveType &getCurve() { return fanCurve; }


protected:
    [[nodiscard]] uint8_t interpolateFromSegment(uint8_t p0, uint8_t p1, const float &tempCentiCelsius) const
    {
        float k, x, d;
        computeLinearCoefficientsFromSegment(p0, p1, tempCentiCelsius, k, x, d);
        return static_cast<uint8_t>(roundf(k * x + d));
    }


    void computeLinearCoefficientsFromSegment(uint8_t p0, uint8_t p1, const float &tempCentiCelsius, float &k, float &x, float &d) const
    {
        const float dy{ static_cast<float>(fanCurve[p1].power - fanCurve[p0].power) };
        const float dx{ static_cast<float>(fanCurve[p1].tempCentiCelsius - fanCurve[p0].tempCentiCelsius) };

        k = dy / dx;
        x = tempCentiCelsius - static_cast<float>(fanCurve[p0].tempCentiCelsius);
        d = fanCurve[0].power;
    }

    uint8_t interpolatedPower{ 0 }; // output range: 0 - 255
    FanCurveType fanCurve{};
};


using FanSpeedInterpolator4Points = FanSpeedInterpolator<4>;
