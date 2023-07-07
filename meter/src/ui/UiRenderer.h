#pragma once

#if defined(HAS_DISPLAY)
#if defined(DISPLAY_I2C)
    #include "ui/i2c/UiRendererU8x8.h"
#elif defined(DISPLAY_PARALLEL_8BIT)
    #include "ui/parallel_8bit/UiRendererTftShieldV3.h"
#else
    #error display not implemented for this build
#endif

#endif // HAS_DISPLAY
