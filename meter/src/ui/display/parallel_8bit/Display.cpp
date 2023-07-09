#include "../Display.h"

#include <Arduino.h>

bool displayInit(display_t &display)
{
    Serial.print("#I display setup:");
    const uint16_t reportedId{ display.readID() };
    if(reportedId != 0)
    {
        Serial.print(" TFT-identifier=");
        Serial.print(reportedId);
        Serial.print(", expected identifier=");
        Serial.print(TFT_EXPECTED_ID);
        Serial.println((reportedId == TFT_EXPECTED_ID) ? " - ok" : " - warning");
        display.begin(reportedId);
        return true;
    }
    Serial.println(" failed to initialize display");
    return false;
}
