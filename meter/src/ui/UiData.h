#pragma once
#include "../lib/si_units/si_unit_type.h"
#include <inttypes.h>

class SampleRegister;
class OperatingState;

enum class CurrentUiContext : uint8_t
{
    Main,
    Setup,
    Default = Main
};

struct UiData
{
    CurrentUiContext uiContext{ CurrentUiContext::Default };
    struct
    {
        uint16_t sample{ 0 };
        float dbmW{ 0 };
        float watt{ 0 };
        SiUnitType si{ SiUnitType::TIMES_ONE };
    } probe;

    struct
    {
        int rawTemp10Bit{ 0 };
        float milliVolt{ 0 };
        float kelvin{ 0 };
        float celsius{ 0 };
    } temperature;
};
