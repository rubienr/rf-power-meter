#include "AD8138.h"
#include <math.h>

namespace ad8138
{

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
    const uint16_t v{rawValue};
    correctedDbmW = k0 + (k1 * v) + (k2 * v * v) + (k3 * v * v * v);
}

void AD8138Converter3rdOrder::convertWatt(const float &correctedDbmW, float &watt, UnitType &siUnit)
{
    const float milliWatt = powf(10.0f, correctedDbmW / 10.0f);

    if(milliWatt < 0.000000001f) // femto Watt
    {
        watt = milliWatt * 1e12f;
        siUnit = UnitType::FEMTO;
    }
    else if(milliWatt < 0.000001f) // pico Watt
    {
        watt = milliWatt * 1e9f;
        siUnit = UnitType::PICO;
    }
    else if(watt < 0.001f) // nano Watt
    {
        watt = milliWatt * 1e6f;
        siUnit = UnitType::NANO;
    }
    else if(watt < 1.0f) // micro Watt
    {
        watt = milliWatt * 1e3f;
        siUnit = UnitType::MICRO;
    }
    else if(milliWatt < 1000.0f) // milli Watt
    {
        // milliWatt *= 10e0f;
        siUnit = UnitType::MILLI;
    }
    else // Watt
    {
        watt = milliWatt * 10e-3f;
        siUnit = UnitType::TIMES_ONE;
    }
}

} // namespace ad8138
