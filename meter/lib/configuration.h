#pragma once

// ----- section: version
#define VERSION_MAJOR                  0
#define VERSION_MINOR                  0
#define VERSION_PATCH                  1

// ----- section: sample
#define SAMPLE_TIMER_MS_DEFAULT        50
#define SAMPLE_TIMER_MS_MAX            1000
#define SAMPLE_TIMER_MS_MIN            30

// ----- section: display
#define RENDER_TIMER_MS_DEFAULT        250
#define RENDER_TIMER_MS_MAX            1000
#define RENDER_TIMER_MS_MIN            100

// ----- section: periphery
// #define ACTIVITY_LED         LED_BUILTIN // enables LED blinking on each render loop; disable in production

// ----- section: storage
#define EEPROM_START_ADDRESS           0x0 // move the start address if EEPROM is broken to avoid damaged segments

// ----- section: AD7887

#define AD7887_CHIP_SELECT_PIN         10 // CS pin
#define AD7887_CLK_PIN                 9  // SCLK
#define AD7887_DOUT_PIN                12 // DOUT
#define AD7887_DIN_PIN                 11 // DIN

// The maximum clock frequency is f_sclk=2MHz, thus a clock-signal hold of t_sclk=500ns is the least appropriate delay.
// Note:
// - the recommended values below are meant vor VDD 4.75V to 5.25V
// - wait for nanoseconds is not possible, thus most delays end in 1µS
#define AD7887_T2_DELAY_US             1 // max 30ns; CS until DOUT three-state disabled; comment out to disable
#define AD7887_T6_DELAY_US             1 // min 0.4*t_sclk; clock high pulse width; comment out to disable
#define AD7887_T7_DELAY_US             1 // min 0.4*t_sclk; clock low pulse width; comment out to disable

#define AD7887_SUBSEQUENT_READ_ERRORS  3 // maximum allowed read errors before op-mode switches to error mode
#define AD7887_SUBSEQUENT_ZERO_SAMPLES 3 // maximum allowed 0x000-valued samples before op-mode switches to error mode
