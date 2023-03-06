#pragma once
#include "resources.h"

struct Firmware : public Resources
{
    void setup();
    void process();

protected:
    void doSample();
    void doRender();
#if defined(AD8318_TEMPERATURE_FEATURE)
    void doSampleTemperature();
#endif

    [[noreturn]] void doResetAndReboot();
#if defined(POWER_OFF_FEATURE) || defined(AUTO_POWER_OFF_FEATURE)
    [[noreturn]] static void doPowerOff();
#endif
    [[noreturn]] void doHalt();

    static void logLoadSettings(const StorageLoadResult &result);
    static void logStoreSettings(const StorageStoreResult &result);

#if defined(ACTIVITY_LED)
    bool toggleActivityLed();
#endif
    bool isSampleTimeout();
    bool isRenderTimeout();
#if defined(AD8318_TEMPERATURE_FEATURE)
    bool isTemperatureTimeout();
#endif
#if defined(AUTO_POWER_OFF_FEATURE)
    bool isAutoPowerOffTimeout();
    void resetAutoPowerOffTimeout();
#endif
};
