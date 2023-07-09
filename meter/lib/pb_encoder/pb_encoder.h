#pragma once
#include "../rfmeter_configuration.h"

#if defined(HAS_ENCODER)

    #if !defined(ENV_NATIVE)
        #include <Arduino.h>
    #else
void attachInterrupt(digitalPinToInterrupt(pin), ISR, mode);
    #endif
    #include <inttypes.h>

typedef struct
{
    volatile int8_t counter;
    uint8_t isPbPressed : 1;
} PbEncoder_Handle;

extern PbEncoder_Handle *pbEncoderHandle;

void PbEncoder_setup();
void PbEncoder_teardown();

#endif // HAS_ENCODER
