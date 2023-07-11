#pragma once

#include "../lib/data_sink/DataSinkRegisters.h"
#include "elapsedMillis.h"
#include "states/OperatingState.h"
#include "ui/display/Display.h"
#include "ui/renderer/UiData.h"
#include "ui/renderer/UiRenderer.h"
#include <Arduino.h>

struct Resources
{
    void (*reboot)(){nullptr};

    OperatingState operatingState{};

    UiData uiData{};

    display_t display DISPLAY_INITIALIZER_LIST;
    UiRenderer renderer{uiData, operatingState, display, Serial};

    struct
    {
        elapsedMillis renderMs{0};
#if defined(AUTO_POWER_OFF_FEATURE)
        elapsedSeconds autoPowerOffSec{0};
#endif
    } timers{};

    struct
    {
        datasink::DataSink data{};
        bool dataUpdated{false};
    } sink;
};
