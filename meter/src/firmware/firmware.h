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

    static void logLoadSettings(const StorageLoadResult &result);

    bool toggleActivityLed();
    bool isSampleTimeout();

    bool isRenderTimeout();
};
