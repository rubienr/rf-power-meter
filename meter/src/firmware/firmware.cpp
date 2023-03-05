#include "firmware.h"
#include "../../lib/settings/log.hpp"
#include "../../lib/storage/SettingsStorage.hpp"

void Firmware::setup()
{
    EarlyInitializer _{}; // TODO: this is 2nd attempt; 1st attempt does not initialize correctly
    Serial.println(F("#D setup ..."));
    Serial.print(F("#I RF Meter Version "));
    Serial.print(VERSION_MAJOR);
    Serial.print('.');
    Serial.print(VERSION_MINOR);
    Serial.print('.');
    Serial.print(VERSION_PATCH);
    Serial.print(F(" built "));;
    Serial.print(__DATE__);
    Serial.print(F(" "));;
    Serial.print(__TIME__);
    Serial.println();

    Serial.println(F("#I Notes:"));
    Serial.println(F("#I   - lines with leading '#' are human readable logs"));
    Serial.println(F("#I   - lines without leading '#' are slightly machine parseable"));
    Serial.println(F("#I   - D=DEBUG, I=INFO, E=ERROR, F=FATAL"));

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 0);
#ifdef ACTIVITY_LED
    pinMode(ACTIVITY_LED, OUTPUT);
#endif

    display.begin();
    display.setPowerSave(0);
    display.setFont(u8x8_font_chroma48medium8_r);
    display.clear();

    auto loadSettingsState = settingsStorage.loadOrInit(settings);
    logLoadSettings(loadSettingsState);
    if(loadSettingsState.fatal_error)
        operatingState.emergency = EmergencyType::HaltOnUnrecoverableStorageError;
    else
    {
        Serial.println(F("{\n  \"settings\" : {"));
        LogSettings logger(Serial);
        logger.log(settings, 2);
        Serial.println(F("  }\n}"));
    }
    Serial.println(F("#D setup: done"));
}

void Firmware::logLoadSettings(const StorageLoadResult &result)
{
    if(result.loaded_crc_mismatch == 1)
        Serial.println(F("#E loaded settings from EEPROM: CRC mismatch"));
    if(result.loaded_version_mismatch == 1)
        Serial.println(F("#W loaded settings from EEPROM: version mismatch"));

    if(result.stored_defaults == 1) Serial.println(F("#D defaults stored"));
    if(result.loaded_defaults == 1) Serial.println(F("#D reloaded defaults"));

    if(result.loaded_crc_mismatch_after_storing_defaults == 1)
        Serial.println(F("#E failed to load settings from EEPROM: CRC mismatch"));
    if(result.loaded_version_mismatch_after_storing_defaults == 1)
        Serial.println(F("#E failed to load settings from EEPROM: version mismatch"));

    if(result.fatal_error == 1)
        Serial.println(F("#F fatal error while loading settings; EEPROM defect?"));
    else Serial.println(F("#I settings loaded successfully from EEPROM"));
}

void Firmware::process()
{
    switch(operatingState.emergency)
    {
    case EmergencyType::None:
        break;
    default:
        doHalt();
        break;
    }

    switch(operatingState.mode)
    {
    case OperatingMode::Measure:
        if(isSampleTimeout()) { doSample(); }
        if(isRenderTimeout()) { doRender(); }
        break;

    case OperatingMode::Idle:
        if(isRenderTimeout()) { doRender(); }
        break;


    default:
        break;
    }
}

void Firmware::doSample() { sampleTimer = 0; }

void Firmware::doRender()
{

#ifndef ACTIVITY_LED
    static uint8_t isOn{ 0 };
    isOn = isOn ? 0 : 1;
#else
    bool isOn{ toggleActivityLed() };
#endif
    display.setCursor(0, 0);
    display.print(isOn ? '+' : ' ');
    display.display();
    Serial.print('.');
    renderTimer = 0;
}

[[noreturn]] void Firmware::doHalt()
{
    renderer.render();
    for(;;) {}
}

#ifdef ACTIVITY_LED
bool Firmware::toggleActivityLed()
{
    static bool isOn{ false };
    isOn = !isOn;
    digitalWrite(ACTIVITY_LED, isOn);
    return isOn;
}
#endif

bool Firmware::isSampleTimeout() { return sampleTimer > settings.sample.separation_ms.get(); }

bool Firmware::isRenderTimeout() { return renderTimer > settings.render.separation_ms.get(); }
