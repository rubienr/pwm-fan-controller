#include <cinttypes>
#include <cmath>


struct CurvePoint
{
    uint8_t power;           // on a scale from 0 to 255
    int16_t tempDeciCelsius; // i.e. 12.34°C corresponds to 123
};


template <uint8_t NumPoints> struct FanSpeedInterpolator
{
    using FanCurveType = CurvePoint[NumPoints];
    static_assert(NumPoints > 1);


    bool setPowerCurvePoints(const uint8_t (&power)[NumPoints], const int16_t (&tempDeciCelsius)[NumPoints])
    {
        // assert: tempDeciCelsius[N] < tempDeciCelsius[N+1]
        for(uint8_t idx{ 1 }; idx < NumPoints; idx++)
            if(tempDeciCelsius[idx - 1] >= tempDeciCelsius[idx]) return false;

        for(uint8_t idx{ 0 }; idx < NumPoints; idx++)
        {
            fanCurve[idx].power = power[idx];
            fanCurve[idx].tempDeciCelsius = tempDeciCelsius[idx];
        }
        return true;
    }


    void interpolatePowerFromTemperature(const float &temperatureCelsius)
    {
        auto reportedDeciCelsius = static_cast<int16_t>(lroundf(temperatureCelsius * 10));

        if(reportedDeciCelsius <= fanCurve[0].tempDeciCelsius) // min power
            interpolatedPower = fanCurve[0].power;
        else if(reportedDeciCelsius > fanCurve[NumPoints - 1].tempDeciCelsius) // max power
            interpolatedPower = fanCurve[NumPoints - 1].power;
        else
        {
            for(uint8_t index{ 1 }; index < NumPoints - 1; index++)
                if(reportedDeciCelsius <= fanCurve[index].tempDeciCelsius)
                {
                    interpolatedPower = interpolateFromSegment(index - 1, index, reportedDeciCelsius);
                    return;
                }
        }
    }


    [[nodiscard]] uint8_t getInterpolatedPower() const { return interpolatedPower; }


    [[nodiscard]] const FanCurveType &getCurve() const { return fanCurve; }


    [[nodiscard]] FanCurveType &getCurve() { return fanCurve; }


protected:
    [[nodiscard]] uint8_t interpolateFromSegment(uint8_t p0, uint8_t p1, const float &tempDeciCelsius) const
    {
        float k, x, d;
        computeLinearCoefficientsFromSegment(p0, p1, tempDeciCelsius, k, x, d);
        return static_cast<uint8_t>(roundf(k * x + d));
    }


    void computeLinearCoefficientsFromSegment(uint8_t p0, uint8_t p1, const float &tempDeciCelsius, float &k, float &x, float &d) const
    {
        const float dy{ static_cast<float>(fanCurve[p1].power - fanCurve[p0].power) };
        const float dx{ static_cast<float>(fanCurve[p1].tempDeciCelsius - fanCurve[p0].tempDeciCelsius) };

        k = dy / dx;
        x = tempDeciCelsius - static_cast<float>(fanCurve[p0].tempDeciCelsius);
        d = fanCurve[0].power;
    }

    uint8_t interpolatedPower{ 0 }; // output range: 0 - 255
    FanCurveType fanCurve{};
};

#include "configuration.h"
using FanSpeedInterpolator4Points = FanSpeedInterpolator<numCurvePoints>;
