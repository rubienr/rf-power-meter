#include "ui/i2c/UiRendererU8x8.h"
#include "states/OperatingState.h"
#include "ui/UiData.h"
//#include <Adafruit_GFX.h>
//#include <Fonts/FreeSans12pt7b.h>
//#include <Fonts/FreeSans9pt7b.h>
//#include <Fonts/FreeSerif12pt7b.h>
//#include <FreeDefaultFonts.h>

UiRenderer::UiRenderer(UiData &uiData, OperatingState &operatingState, display_t &outDisplay, Stream &outSerial) :
data(uiData), operatingState(operatingState), display(outDisplay), serial(outSerial)
{
    if(!displayInit(display)) { operatingState.setEmergency(EmergencyType::HaltOnDisplayInitError); }
}

void UiRenderer::init()
{
    if(!displayInit(display)) { operatingState.setEmergency(EmergencyType::HaltOnDisplayInitError); }

    display.setPowerSave(0);
    display.setFont(u8x8_font_chroma48medium8_r);
    display.setCursor(0, 0);
    display.clear();
}

void UiRenderer::render()
{
    display.setCursor(0, 0);
    display.print(F("V="));
    display.println(data.probe.dbmW);
    display.display();
}
