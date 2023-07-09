#pragma once
#include "../lib/rfmeter_configuration.h"
#include <HardwareSerial.h>

/// Note: Be careful to call init() once,
/// so that copy elision does not optimize the whole instance away.
struct EarlyInitializer
{
    EarlyInitializer();
    void init();
};
