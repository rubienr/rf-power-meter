#pragma once
#include "../Display.h"

struct UiData;
class Stream;
class OperatingState;

struct UiRenderer
{
public:
    UiRenderer(UiData &uiData, OperatingState &operatingState, display_t &outDisplay, Stream &outSerial);
    void init();
    void render();

private:
    UiData &data;
    OperatingState &operatingState;
    display_t &display;
    Stream &serial;
};
