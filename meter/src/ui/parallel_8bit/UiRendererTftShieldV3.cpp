#include "ui/parallel_8bit/UiRendererTftShieldV3.h"
#include "ui/UiData.h"
#include "states/OperatingState.h"
#include <MCUFRIEND_kbv.h>


UiRenderer::UiRenderer(UiData &uiData, OperatingState &operatingState, display_t &outDisplay, Stream &outSerial) :
data(uiData), operatingState(operatingState), display(outDisplay), serial(outSerial)
{
}

void UiRenderer::init()
{
    display.setRotation(0);
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.fillScreen(TFT_BLACK);
    display.setTextColor(TFT_CYAN, TFT_BLACK);
    Serial.println("#I renderer initialized");
}

void UiRenderer::render()
{
    display.setCursor(0, 0);

    display.setTextSize(1);
    display.print("UIS=");
    display.print(static_cast<uint8_t>(data.uiContext));

    display.print(" OPS=");
    display.print(static_cast<uint8_t>(operatingState.getMode()));

    display.print(" TEMP=");
    display.print(data.temperature.kelvin);
    display.println("C");

    display.setTextSize(5);
    display.println();
    display.print(data.probe.dbmW);
    display.setTextSize(2);
    display.println(" dBmW");
    display.setTextSize(5);

    if(data.probe.sample <= 0) return;
    display.println();
    display.setTextSize(5);
    display.print(data.probe.watt);
    display.setTextSize(2);
    display.print(' ');
    display.print(siUnitTypeToStr(data.probe.si));
    display.println("W");
}
