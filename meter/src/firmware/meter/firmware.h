#pragma once
#include "resources.h"

struct Firmware : public Resources
{
    void setup();
    void process();

protected:
    void doSamplePowerMeter();
#if defined(HAS_DATA_SINK_I2C)
    void doSendCapturedData();
#endif // HAS_DATA_SINK_I2C
    void doRender();
#if defined(AD8318_TEMPERATURE_FEATURE)
    void doSampleTemperature();
#endif // AD8318_TEMPERATURE_FEATURE

    [[noreturn]] void doResetAndReboot();
#if defined(POWER_OFF_FEATURE) || defined(AUTO_POWER_OFF_FEATURE)
    [[noreturn]] static void doPowerOff();
#endif // POWER_OFF_FEATURE || AUTO_POWER_OFF_FEATURE
    [[noreturn]] void doHalt();

    static void logLoadSettings(const storage::LoadResult &result);
    static void logStoreSettings(const storage::StoreResult &result);

#if defined(ACTIVITY_LED)
    bool toggleActivityLed();
#endif // ACTIVITY_LED
    bool isPowerMeterSampleTimeout();

#if defined(HAS_DISPLAY)
    bool isRenderTimeout();
#endif // HAS_DISPLAY

#if defined(HAS_DATA_SINK_I2C)
    bool isDataSinkTimeout();
#endif // HAS_DATA_SINK_I2C

#if defined(AD8318_TEMPERATURE_FEATURE)
    bool isTemperatureSampleTimeout();
#endif // AD8318_TEMPERATURE_FEATURE

#if defined(AUTO_POWER_OFF_FEATURE)
    bool isAutoPowerOffTimeout();
    void resetAutoPowerOffTimeout();
#endif // AUTO_POWER_OFF_FEATURE

#if defined(HAS_DATA_SINK_I2C)
    bool initI2cDataSink();
#endif // HAS_DATA_SINK_I2C
    bool initPowerMeterProbe();
    bool initEeprom();
#if defined(HAS_DISPLAY)
    bool initDisplay();
#endif // HAS_DISPLAY
    bool initActivityLed();
};
