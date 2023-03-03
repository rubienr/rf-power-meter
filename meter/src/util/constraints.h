#pragma once

#ifdef ENV_NATIVE
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#else
#include <Arduino.h>
#endif
