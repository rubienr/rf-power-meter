#pragma once
#include "resources.h"

struct Firmware : public Resources
{
    void setup();
    void process();

protected:
    void doSample();
    void doRender();
    [[noreturn]] void doHalt();

    bool toggleActivityLed();
    bool isSampleTimeout();
    bool isRenderTimeout();
};
