#pragma once
#include "../../lib/ad7887/AD7887.h"
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

    Settings settings{};
    SettingsStorage<EepromStorageDevice<Settings>> settingsStorage{};
    OperatingState operatingState{};

    UiData uiData{};
    // U8G2_SSD1306_128X64_NONAME_F_HW_I2C display{ U8G2_R0, U8X8_PIN_NONE, SCL, SDA };
    U8X8_SSD1306_128X64_NONAME_HW_I2C display{ U8X8_PIN_NONE, SCL, SDA };
    UiRenderer renderer{ uiData, operatingState, settingsStorage, display, Serial };

    ControlRegister rfProbeCtlRegister{ .dontCare = 0,
                                        .mustBeZero1 = 0,
                                        .onChipReference = 1,
                                        .singleDualChanelSelect = 0,
                                        .channelSelect = 0,
                                        .mustBeZero2 = 0,
                                        .powerManagement1 = 0,
                                        .powerManagement0 = 1 };
    AD7887 rfProbe{ rfProbeCtlRegister, chipSelectDigitalWrite, clkDigitalWrite, dataDigitalWrite, dataDigitalRead, delayMicroseconds };
    SampleRegister rfSampleRegister{ .zero = 0, .data = 0 };

    elapsedMillis sampleTimer{ 0 };
    elapsedMillis renderTimer{ 0 };
};
