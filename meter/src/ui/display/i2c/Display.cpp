#include "../Display.h"

#include <Arduino.h>

bool displayInit(display_t &display)
{
    Serial.print("#I display setup:");
    if(display.begin())
    {
        Serial.println(" ok");
        return true;
    }
    Serial.println(" failed to initialize display");
    return false;
}
