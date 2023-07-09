#pragma once

#include "data_sink/DataSinkRegisters.h"
#include "elapsedMillis.h"
#include "states/OperatingState.h"
#include "ui/display/Display.h"
#include "ui/renderer/UiData.h"
#include "ui/renderer/UiRenderer.h"
#include <Arduino.h>

struct Resources
{
    OperatingState operatingState{};

    UiData uiData{.uiContext = CurrentUiContext::Default,
                  .probe = {.rawSample = 0, .dbMilliW = 0, .watt = 0, .wattScale = UnitType::FEMTO},
                  .temperature = {0, 0, 0}};

    display_t display DISPLAY_INITIALIZER_LIST;
    UiRenderer renderer{uiData, operatingState, display, Serial};

    struct
    {
        elapsedMillis renderMs{0};
#if defined(AUTO_POWER_OFF_FEATURE)
        elapsedSeconds autoPowerOffSec{0};
#endif
    } timers{};

    datasink::DataSink sink;
};
