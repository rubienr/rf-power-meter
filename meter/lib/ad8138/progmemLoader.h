#pragma once
#if !defined(ENV_NATIVE)
    #include <Arduino.h>
#else
float pgm_read_float_near(float *) { return 0.0f; }
#endif

#include "KValues3rdOrder.hpp"

namespace ad8138
{

template <typename T> struct KValues3rdOrderLoader
{
    static void load(const T *progMemAddress, KValues3rdOrder<T> &kValues)
    {
        kValues.k0 = pgm_read_float_near(&progMemAddress[0]);
        kValues.k1 = pgm_read_float_near(&progMemAddress[1]);
        kValues.k2 = pgm_read_float_near(&progMemAddress[2]);
        kValues.k3 = pgm_read_float_near(&progMemAddress[3]);
    }
};

} // namespace ad8138
