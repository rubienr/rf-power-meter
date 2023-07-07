#pragma once
#include <inttypes.h>

enum class SiUnitType : uint8_t
{
    KILO,      // 10^3
    TIMES_ONE, // 10^0
    MILLI,     // 10^-3
    MICRO,     // 10^-6
    NANO,      // 10^-9
    PICO,      // 10^-12
    FEMTO,     // 10^-15
};

const char siUnitTypeToStr(SiUnitType t);
