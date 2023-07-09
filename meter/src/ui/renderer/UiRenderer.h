#pragma once
#include "../lib/rfmeter_configuration.h"

#if defined(HAS_DISPLAY)

struct UiData;
class Stream;
struct OperatingState;

struct UiRenderer
{
    UiRenderer(UiData &uiData, OperatingState &operatingState, display_t &outDisplay, Stream &outSerial);
    void init();
    void render();

private:
    UiData &data;
    OperatingState &operatingState;
    display_t &display;
    Stream &serial;
};

#endif // HAS_DISPLAY
