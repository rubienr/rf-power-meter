#pragma once

#include "elapsedMillis.h"
#include "states/OperatingState.h"
#include "ui/Display.h"
#include "ui/UiData.h"
#include "ui/UiRenderer.h"
#include <Arduino.h>
#include "data_sink/DataSinkRegisters.h"

struct Resources
{
    OperatingState operatingState{};

    UiData uiData{ .uiContext = CurrentUiContext::Default,
                   .probe = { .sample = 0, .dbmW = 0, .watt = 0, .si = SiUnitType::FEMTO },
                   .temperature = { 0, 0, 0 } };

    display_t display DISPLAY_INITIALIZER_LIST;
    UiRenderer renderer{ uiData, operatingState, display, Serial };

    struct
    {
        elapsedMillis renderMs{ 0 };
#if defined(AUTO_POWER_OFF_FEATURE)
        elapsedSeconds autoPowerOffSec{ 0 };
#endif
    } timers{};

    datasink::DataSink sink;
};
