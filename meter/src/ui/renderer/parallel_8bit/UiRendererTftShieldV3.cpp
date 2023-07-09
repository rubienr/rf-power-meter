#include "../../display/Display.h"
#include "../UiData.h"
#include "../UiRenderer.h"
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

    display.print(" T=");
    display.print(data.temperature.celsius_em2 / 100);
    display.print('.');
    display.print((data.temperature.celsius_em2 % 100) / 10);
    display.println("C   ");

    display.setTextSize(5);
    display.println();
    display.print(data.probe.dbMilliW);
    display.setTextSize(2);
    display.println(" dBmW");
    display.setTextSize(5);

    display.println();
    display.setTextSize(5);
    display.print(data.probe.watt);
    display.setTextSize(2);
    display.print(' ');
    display.print(si::unitTypeToStr(data.probe.wattScale));
    display.println("W");
}
