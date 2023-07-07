#pragma once

#if defined(POWER_METER)
    #include "meter/resources.h"
#elif defined(OFFBOARD_DISPLAY)
    #include "display/resources.h"
#else
    #error
#endif
