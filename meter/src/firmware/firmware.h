#pragma once
#include "EarlyInitializer.h"

#if defined(POWER_METER)
    #include "meter/firmware.h"
#elif defined(OFFBOARD_DISPLAY)
    #include "display/firmware.h"
#else
    #error
#endif
