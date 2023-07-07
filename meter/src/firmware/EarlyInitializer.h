#pragma once
#include <HardwareSerial.h>
#include "configuration.h"

/**
 * Note: Be careful to call init() once,
 * so that copy elision does not optimize the whole instance away.
 */
struct EarlyInitializer
{
    EarlyInitializer();
    void init();
};
