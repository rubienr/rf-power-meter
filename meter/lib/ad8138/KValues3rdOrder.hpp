#pragma once
#if !defined(ENV_NATIVE)
    #include <Arduino.h>
#else
pgm_read_float_near(float *) {}
#endif

template <typename T> struct KValues3rdOrder
{
    T k0{ 0 };
    T k1{ 0 };
    T k2{ 0 };
    T k3{ 0 };
};
