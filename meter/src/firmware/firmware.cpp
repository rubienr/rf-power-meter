#include "firmware.h"
#include "../../lib/storage/SettingsStorage.hpp"

void Firmware::setup()
{
    EarlyInitializer _{};
    Serial.println(F("setup ..."));

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 0);
#ifdef ACTIVITY_LED
    pinMode(ACTIVITY_LED, OUTPUT);
#endif

    display.begin();
    display.setPowerSave(0);
    display.setFont(u8x8_font_chroma48medium8_r);
    display.clear();

    auto transactionResult = settingsStorage.loadOrInit(settings);
    if(transactionResult.fatal_error)
        operatingState.emergency = EmergencyType::HaltOnUnrecoverableStorageError;
    Serial.println(F("setup: done"));
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
    Serial.print(isOn ? '+' : ' ');
    // display.setDrawColor(isOn);
    // display.drawPixel(0, 0);
    //  display.setDrawColor(1);
    // display.sendBuffer();
    display.display();
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
