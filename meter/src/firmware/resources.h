#pragma once
#include <U8x8lib.h>
// #include "U8g2lib.h"
#include "../../lib/settings/Settings.h"
#include "../lib/storage/EepromStorageDevice.hpp"
#include "../lib/storage/SettingsStorage.hpp"
#include "elapsedMillis.h"
#include "states/OperatingState.h"
#include "ui/UiData.h"
#include "ui/UiRendererU8x8.h"
#include <Arduino.h>
#include <HardwareSerial.h>

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

            Serial.println(F("\n\n\n"));
            Serial.println(F("early init: done"));
        }
    } _{};

    Settings settings{};
    SettingsStorage<EepromStorageDevice<Settings>> settingsStorage{};
    OperatingState operatingState{};

    UiData uiData{};
    // U8G2_SSD1306_128X64_NONAME_F_HW_I2C display{ U8G2_R0, U8X8_PIN_NONE, SCL, SDA };
    U8X8_SSD1306_128X64_NONAME_HW_I2C display{ U8X8_PIN_NONE, SCL, SDA };
    UiRenderer renderer{ uiData, operatingState, settingsStorage, display, Serial };

    elapsedMillis sampleTimer{ 0 };
    elapsedMillis renderTimer{ 0 };
};
