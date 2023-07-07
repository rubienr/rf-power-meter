#pragma once
#include "./ad7887/AD7887.h"
#include "./ad8138/AD8138.h"
#include "./ad8138/progmemLoader.h"
#include "./settings/Settings.h"
#include "./storage/EepromStorageDevice.hpp"
#include "./storage/SettingsStorage.hpp"
#include "elapsedMillis.h"
#include "states/OperatingState.h"
#include "ui/Display.h"
#include "ui/UiData.h"
#include "ui/UiRenderer.h"
#include <Arduino.h>
#include <HardwareSerial.h>

extern const float kValuesSets[6][4] PROGMEM;

void clkDigitalWrite(uint8_t digitalValue);
void chipSelectDigitalWrite(uint8_t digitalValue);
void dataDigitalWrite(uint8_t digitalValue);
int dataDigitalRead();

struct Resources
{
    OperatingState operatingState{};

    struct
    {
        Settings parameters{};
        SettingsStorage<EepromStorageDevice<Settings>> storage{};
    } settings{};

    UiData uiData{ .uiContext = CurrentUiContext::Default,
                   .probe = { .sample = 0, .dbmW = 0, .watt = 0, .si = SiUnitType::FEMTO },
                   .temperature = { 0, 0, 0 } };


#if defined(HAS_DISPLAY)
    display_t display DISPLAY_INITIALIZER_LIST;
    UiRenderer renderer{ uiData, operatingState, display, Serial };
#endif // HAS_DISPLAY

    struct
    {
        ControlRegister ctlRegister{ .powerManagement0 = 1,
                                     .powerManagement1 = 0,
                                     .mustBeZero1 = 0,
                                     .channelSelect = 0,
                                     .singleDualChanelSelect = 0,
                                     .onChipReference = 1,
                                     .mustBeZero2 = 0,
                                     .dontCare = 0 };
        AD7887 device{ ctlRegister, chipSelectDigitalWrite, clkDigitalWrite, dataDigitalWrite, dataDigitalRead, delayMicroseconds };
        SampleRegister sampleRegister{ .data = 0, .zero = 0 };

        KValues3rdOrderFloat kValues{};
        AD8138Converter3rdOrder converter{ kValues };
    } probe{};

    struct
    {
        elapsedMillis sampleMs{ 0 };
#if defined(HAS_DISPLAY)
        elapsedMillis renderMs{ 0 };
#endif // HAS_DISPLAY
#if defined(HAS_DATA_SINK_I2C)
        elapsedMillis dataSinkMs{ 0 };
#endif // HAS_DATA_SINK_I2C
#if defined(AD8318_TEMPERATURE_FEATURE)
        elapsedMillis temperatureMs{ 0 };
#endif // AD8318_TEMPERATURE_FEATURE
#if defined(AUTO_POWER_OFF_FEATURE)
        elapsedSeconds autoPowerOffSec{ 0 };
#endif // AUTO_POWER_OFF_FEATURE
    } timers{};
};

