#include "UiRendererU8x8.h"
#include "UiData.h"
#include "states/OperatingState.h"
#include <U8x8lib.h>


UiRenderer::UiRenderer(UiData &uiData, OperatingState &operatingState, SettingsStorage<EepromStorageDevice<Settings>> &settings, U8X8 &outDisplay, Stream &outSerial) :
data(uiData), operatingState(operatingState), settings(settings), display(outDisplay), serial(outSerial)
{
}

void UiRenderer::render()
{
    if(operatingState.emergency != EmergencyType::None)
    {
        serial.print("#W EmergencyType=");
        serial.println(emergencyTypeToStr(operatingState.emergency));
    }
}
