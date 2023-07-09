#pragma once
#if !defined(ENV_NATIVE)
    #include <Arduino.h>
#else
float pgm_read_float_near(float *) { return 0.0f; }
#endif

namespace ad8138
{

template <typename T> struct KValues3rdOrder
{
    T k0{0};
    T k1{0};
    T k2{0};
    T k3{0};
};

} // namespace ad8138
