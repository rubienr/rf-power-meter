#pragma once
#include "../si_units/si_unit_type.h"
#include "KValues3rdOrder.hpp"
#include <inttypes.h>

using KValues3rdOrderFloat = KValues3rdOrder<float>;

struct AD8138Converter3rdOrder
{
    explicit AD8138Converter3rdOrder(const KValues3rdOrderFloat &kValues, uint8_t attenuationDb = 0);

    void getCorrectionCoefficients(KValues3rdOrderFloat &kValues);
    void setCorrectionCoefficients(const KValues3rdOrderFloat &kValues);

    void setAttenuationDb(int8_t newAttenuationDb);
    int8_t getAttenuationDb() const;

    /**
     * Converts the raw value to dBmW according to the correction coefficients (k-values) and attenuation offset.
     * @param rawValue the value to convert
     * @param correctedDbmW converted and corrected value
     */
    void convertDbMilliWatt(uint16_t rawValue, float &correctedDbmW) const;

    /**
     * Converts the dBmW value to Watts using SI units.
     * @param correctedDbmW the dBmW value to convert to W
     * @param watt converted W value
     * @param siPrefix respective SI unit
     */
    void convertWatt(const float &correctedDbmW, float &watt, SiUnitType &siPrefix);

protected:
    float k0{ 0 };
    float k1{ 0 };
    float k2{ 0 };
    float k3{ 0 };
    int8_t attenuationDb{ 0 };
};
