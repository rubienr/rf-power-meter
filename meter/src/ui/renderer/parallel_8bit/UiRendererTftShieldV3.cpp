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
    // display.reset();
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
    display.print("UiP=");
    display.print(uiScreenToStr(data.ui.page));
    display.print(" UiI=");
    display.print(data.ui.item);

    display.print(" OpSt=");
    display.print(operatingStateToStr(operatingState.getMode()));

    display.print(" T=");
    display.print(data.temperature.celsius_em2 / 100);
    display.print('.');
    display.print((abs(data.temperature.celsius_em2) % 100u) / 10u);
    display.println("C   ");

    display.print("Tse=");
    display.print(data.temperature.sampleSeparationMs_ep1 * 10u);

    display.print(" Sse=");
    display.print(data.power.sampleSeparationMs_ep1 * 10u);

    display.print(" Sav=");
    display.print((data.power.averageSamplesCount_1_to_32 == 0) ? 32 : data.power.averageSamplesCount_1_to_32);
    display.print("  ");

    display.setTextSize(5);
    display.println();
    if(data.power.dbMilliW >= 0.0f)
    {
        display.print(" ");
        display.print(data.power.dbMilliW);
    }
    else { display.print(data.power.dbMilliW); }
    display.setTextSize(2);
    display.println(" dBmW");
    display.setTextSize(5);

    display.println();
    display.print(data.power.watt_em4 / 10);
    display.print('.');
    display.print(data.power.watt_em4 % 10u);

    display.setTextSize(2);
    display.print(' ');
    display.print(si::unitTypeToChar(data.power.wattScale));
    display.println("W    ");
}
