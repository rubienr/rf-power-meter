#pragma once
#include "../../lib/ad7887/AD7887.h"
#include "../../lib/ad8138/AD8138.h"
#include "../../lib/ad8138/progmemLoader.h"
#include "../../lib/settings/Settings.h"
#include "../lib/storage/EepromStorageDevice.hpp"
#include "../lib/storage/SettingsStorage.hpp"
#include "elapsedMillis.h"
#include "states/OperatingState.h"
#include "ui/UiData.h"
#include "ui/UiRendererU8x8.h"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <U8x8lib.h>

extern const float kValuesSets[6][4] PROGMEM;

void clkDigitalWrite(uint8_t digitalValue);
void chipSelectDigitalWrite(uint8_t digitalValue);
void dataDigitalWrite(uint8_t digitalValue);
int dataDigitalRead();

struct Resources
{
    struct EarlyInitializer
    {
        EarlyInitializer()
        {
            Serial.begin(115200, SERIAL_8N1);
            while(!Serial)
            {
                delay(10);
                Serial.begin(115200, SERIAL_8N1);
            }
            Serial.println(F("\n#D early init: done"));
        }
    } _{};

    OperatingState operatingState{};

    struct
    {
        Settings parameters{};
        SettingsStorage<EepromStorageDevice<Settings>> storage{};
    } settings{};

    UiData uiData{};
    // U8G2_SSD1306_128X64_NONAME_F_HW_I2C display{ U8G2_R0, U8X8_PIN_NONE, SCL, SDA };
    U8X8_SSD1306_128X64_NONAME_HW_I2C display{ U8X8_PIN_NONE, SCL, SDA };
    UiRenderer renderer{ uiData, operatingState, settings.storage, display, Serial };

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
        elapsedMillis renderMs{ 0 };
#if defined(AD8318_TEMPERATURE_FEATURE)
        elapsedMillis temperatureMs{ 0 };
#endif
#if defined(AUTO_POWER_OFF_FEATURE)
        elapsedSeconds autoPowerOffSec{ 0 };
#endif
    } timers{};
};
