#pragma once
#include "../configuration.h"

#if defined(HAS_DISPLAY)
    #if defined(DISPLAY_I2C)
        #include <U8x8lib.h>
using display_t = U8X8_SSD1306_128X64_NONAME_HW_I2C;
    #elif defined(DISPLAY_PARALLEL_8BIT)
        #include <MCUFRIEND_kbv.h>
using display_t = MCUFRIEND_kbv;
    #else
        #error
    #endif

bool displayInit(display_t &display);
#endif // HAS_DISPLAY
