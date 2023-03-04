#include "UiRendererU8x8.h"
#include "../../lib/settings/Settings.h"
#include "UiData.h"
#include "states/OperatingState.h"
#include <HardwareSerial.h>
#include <U8g2lib.h>

UiRenderer::UiRenderer(UiData &uiData,
                       OperatingState &operatingState,
                       SettingsStorage<EepromStorageDevice<Settings>> &settings,
                       U8X8 &outDisplay,
                       Stream &outSerial) :
data(uiData),
operatingState(operatingState), settings(settings), display(outDisplay), serial(outSerial)
{
}

void UiRenderer::render()
{
    if(operatingState.emergency != EmergencyType::None)
    {
        Serial.print("# EmergencyType=");
        Serial.println(emergencyTypeToStr(operatingState.emergency));
    }
}
