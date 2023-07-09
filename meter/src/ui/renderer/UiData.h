#pragma once


#include "../lib/rfmeter_configuration.h"

#if defined(HAS_DISPLAY)
    #include "../lib/si_units/si_unit_type.h"
    #include <inttypes.h>

using namespace si;

enum class CurrentUiContext : uint8_t
{
    Main,
    Setup,
    Default = Main
};

struct UiData
{
    CurrentUiContext uiContext{CurrentUiContext::Default};
    struct
    {
        uint16_t rawSample12Bit{0};
        uint16_t rawAverage12Bit{0};
        float dbMilliW{0};
        float watt{0};
        UnitType wattScale{UnitType::TIMES_ONE};
    } probe;

    struct
    {
        uint16_t rawTemp10Bit{0};
        uint16_t volt_em4{0};
        uint16_t kelvin_em2{0};
        int16_t celsius_em2{0};
    } temperature;
};

#endif // HAS_DISPLAY
