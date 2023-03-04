#pragma once

// ----- section: version
#define VERSION_MAJOR           0
#define VERSION_MINOR           0
#define VERSION_PATCH           1

// ----- section: sample
#define SAMPLE_TIMER_MS_DEFAULT 50
#define SAMPLE_TIMER_MS_MAX     1000
#define SAMPLE_TIMER_MS_MIN     30

// ----- section: display
#define RENDER_TIMER_MS_DEFAULT 250
#define RENDER_TIMER_MS_MAX     1000
#define RENDER_TIMER_MS_MIN     100

// ----- section: periphery
#define ACTIVITY_LED            LED_BUILTIN // enables LED blinking on each render loop; disable in production

// ----- section: storage
#define EEPROM_START_ADDRESS    0x0 // change if EEPROM is broken to avoid damaged sements
