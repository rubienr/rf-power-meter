#include "UiRendererU8x8.h"

UiRendererU8X8::UiRendererU8X8(UiData &uiData, U8X8 &outDisplay, Stream &outSerial)
: data(uiData), display(outDisplay), serial(outSerial)
{
}
