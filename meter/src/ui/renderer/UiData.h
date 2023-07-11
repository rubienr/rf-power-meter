#pragma once


#include "../lib/rfmeter_configuration.h"

#if defined(HAS_DISPLAY)
    #include "../lib/si_units/si_unit_type.h"
    #include <inttypes.h>

using namespace si;

enum class UiScreen : uint8_t
{
    Main = 1,
    Default = Main,
    Info,
    Config,
    Undefined
};

constexpr const char *uiScreenToStr(UiScreen s)
{
    switch(s)
    {
    case UiScreen::Main:
        return "Main";
    case UiScreen::Info:
        return "Info";
    case UiScreen::Config:
        return "Config";
    default:
        return "?";
    }
}

constexpr UiScreen uiScreenFromUnderlyingType(uint8_t s)
{
    return (static_cast<uint8_t>(UiScreen::Main) <= s && s <= static_cast<uint8_t>(UiScreen::Undefined)) ? static_cast<UiScreen>(s) :
                                                                                                           UiScreen::Undefined;
}

struct UiData
{

    struct
    {
        float dbMilliW{0};
        uint16_t watt_em4{0};
        UnitType wattScale{UnitType::TimesOne};
        uint8_t averageSamplesCount_1_to_32{0};
        uint8_t sampleSeparationMs_ep1{0};
    } power;

    struct
    {
        uint16_t kelvin_em2{0};
        int16_t celsius_em2{0};
        uint8_t sampleSeparationMs_ep1{0};
    } temperature;

    struct
    {
        UiScreen page{UiScreen::Default};
        uint8_t item{0};
    } ui;
};

#endif // HAS_DISPLAY
