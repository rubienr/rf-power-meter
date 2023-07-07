#pragma once
#include "resources.h"

struct Firmware : public Resources
{
    void setup();
    void process();
    void onReadFromSinkEvent();
    void onWriteToSinkEvent(int count);

protected:
    void doRender();

    [[noreturn]] void doHalt();

    bool toggleActivityLed();
    bool isRenderTimeout();
    bool initI2cDataSource();
    bool initDisplay();
    bool initActivityLed();
};
