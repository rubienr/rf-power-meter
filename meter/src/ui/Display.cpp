#include "Display.h"

#if defined(HAS_DISPLAY)
    #include <Arduino.h>

bool displayInit(display_t &display)
{
    Serial.print("#I display setup:");
    #if defined(DISPLAY_I2C)
    if(display.begin())
    {
        Serial.println(" ok");
        return true;
    }
    Serial.println(" failed to initialize display");
    return false;
    #elif defined(DISPLAY_PARALLEL_8BIT)
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
    #else
        #error
    #endif
    return false;
}

#endif // HAS_DISPLAY
