#include "EarlyInitializer.h"
#include <Arduino.h>

#define STRINGIZE0(x) #x
#define STRINGIZE(x)  STRINGIZE0(x)

EarlyInitializer::EarlyInitializer() { init(); }

void EarlyInitializer::init()
{
    Serial.end();
    Serial.begin(SERIAL_LOG_BAUD_RATE, SERIAL_LOG_CONN_MODE);
    while(!Serial)
    {
        delay(10);
        Serial.begin(SERIAL_LOG_BAUD_RATE, SERIAL_LOG_CONN_MODE);
    }
    Serial.println(
    F("\n#D early init: baud=" STRINGIZE(SERIAL_LOG_BAUD_RATE) " config=" STRINGIZE(SERIAL_LOG_CONN_MODE) " done"));
}

#undef STRINGIZE
#undef STRINGIZE0
