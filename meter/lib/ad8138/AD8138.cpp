#include "AD8138.h"
#include <math.h>

AD8138Converter3rdOrder::AD8138Converter3rdOrder(const KValues3rdOrderFloat &kValues, uint8_t attenuationDb) :
k0(kValues.k0), k1(kValues.k1), k2(kValues.k2), k3(kValues.k3), attenuationDb(attenuationDb)
{
}

void AD8138Converter3rdOrder::getCorrectionCoefficients(KValues3rdOrderFloat &kValues)
{
    kValues.k0 = k0;
    kValues.k1 = k1;
    kValues.k2 = k2;
    kValues.k3 = k3;
}

void AD8138Converter3rdOrder::setCorrectionCoefficients(const KValues3rdOrderFloat &kValues)
{
    k0 = kValues.k0;
    k1 = kValues.k1;
    k2 = kValues.k2;
    k3 = kValues.k3;
}

void AD8138Converter3rdOrder::setAttenuationDb(int8_t newAttenuationDb) { attenuationDb = newAttenuationDb; }

int8_t AD8138Converter3rdOrder::getAttenuationDb() const { return attenuationDb; }

void AD8138Converter3rdOrder::convertDbMilliWatt(uint16_t rawValue, float &correctedDbmW) const
{
    const uint16_t v{ rawValue };
    correctedDbmW = k0 + (k1 * v) + (k2 * v * v) + (k3 * v * v * v);
}

void AD8138Converter3rdOrder::convertWatt(const float &correctedDbmW, float &watt, SiUnitType &siUnit)
{
    const float milliWatt = pow(10.0f, correctedDbmW / 10.0f);

    if(milliWatt < 0.000000001f) // femto Watt
    {
        watt = milliWatt * 1e12;
        siUnit = SiUnitType::FEMTO;
    }
    else if(milliWatt < 0.000001f) // pico Watt
    {
        watt = milliWatt * 1e9;
        siUnit = SiUnitType::PICO;
    }
    else if(watt < 0.001f) // nano Watt
    {
        watt = milliWatt * 1e6;
        siUnit = SiUnitType::NANO;
    }
    else if(watt < 1.0f) // micro Watt
    {
        watt = milliWatt * 1e3;
        siUnit = SiUnitType::MICRO;
    }
    else if(milliWatt < 1000.0f) // milli Watt
    {
        // milliWatt *= 10e0f;
        siUnit = SiUnitType::MILLI;
    }
    else
    {
        watt = milliWatt * 10e-3f;
        siUnit = SiUnitType::TIMES_ONE;
    }
}
