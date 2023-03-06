#pragma once

// ----- section: version

#define VERSION_MAJOR            0
#define VERSION_MINOR            0
#define VERSION_PATCH            1

// ----- section: sample

#define SAMPLE_TIMER_MS_DEFAULT  50
#define SAMPLE_TIMER_MS_MAX      1000
#define SAMPLE_TIMER_MS_MIN      30

// ----- section: display

#define RENDER_TIMER_MS_DEFAULT  250
#define RENDER_TIMER_MS_MAX      1000
#define RENDER_TIMER_MS_MIN      100

// ----- section: periphery

// #define ACTIVITY_LED         LED_BUILTIN // enables LED blinking on each render loop; disable in production

// ----- section: storage

#define EEPROM_START_ADDRESS     0x0 // move the start address if EEPROM is broken to avoid damaged segments
#define EEPROM_RESET_PIN_FEATURE     // enable/disable manual EEPROM reset feature
#if defined(EEPROM_RESET_PIN_FEATURE)
    #define EEPROM_RESET_PIN               6   // input pin that triggers reset
    #define EEPROM_RESET_PIN_TRIGGER_LEVEL LOW // input level that triggers reset
    #define EEPROM_RESET_PIN_DELAY_MS      250 // milliseconds delay from pin being "released" to power off
#endif

// ----- section: AD7887

#define AD7887_CHIP_SELECT_PIN        10 // CS pin - chip select on AD7887
#define AD7887_CLK_PIN                9  // SCLK - clock to AD7887
#define AD7887_DOUT_PIN               12 // DOUT - data from AD7887
#define AD7887_DIN_PIN                11 // DIN - data to AD7887

// The maximum clock frequency is f_sclk=2MHz, thus a clock-signal hold of t_sclk=500ns is the least appropriate delay.
// Note:
// - the recommended values below are meant vor VDD 4.75V to 5.25V
// - wait for nanoseconds is not possible, thus most delays end in 1µS
#define AD7887_T2_DELAY_US            1 // max 30ns; CS until DOUT three-state disabled; comment out to disable
#define AD7887_T6_DELAY_US            1 // min 0.4*t_sclk; clock high pulse width; comment out to disable
#define AD7887_T7_DELAY_US            1 // min 0.4*t_sclk; clock low pulse width; comment out to disable

#define AD7887_SUBSEQUENT_READ_ERRORS 3 // maximum allowed read errors before op-mode switches to error mode
// #define AD7887_SUBSEQUENT_ZERO_SAMPLES 3 // maximum allowed 0x000-valued samples before op-mode switches to error mode

// ----- section: AD8318

#define AD8318_TEMPERATURE_FEATURE
#if defined(AD8318_TEMPERATURE_FEATURE)
    #define AD8318_TEMPERATURE_SAMPLE_MS         1000 // separation in between temperature measurements
    #define AD8318_TEMPERATURE_SAMPLE_MS_MIN     500  // min configurable separation
    #define AD8318_TEMPERATURE_SAMPLE_MS_MAX     5000 // max configurable separation
    #define AD8318_TEMPERATURE_PIN               A0   // analog temperature output
    #define AD8318_TEMPERATURE_MILLI_VOLT_KELVIN 2.0  // 2mV/°K; about 600mV at 27°C (273.15°K+27°K)

    // see:
    // -`analogReference()`
    // - https://reference.arduino.cc/reference/en/language/functions/analog-io/analogreference/
    #if defined(ARDUINO_AVR_UNO)                           // mcu=atmega328p
        #define AD8318_TEMPERATURE_REF_CONFIG  EXTERNAL    // default: INTERNAL; the 1.1V ref. may not be as accurate as expected
        #define AD8318_TEMPERATURE_REF_VOLTAGE 3.3         // default: 1.1
    #elif defined(ARDUINO_AVR_MEGA2560)                    // mcu=atmega2560
        #define AD8318_TEMPERATURE_REF_CONFIG  INTERNAL1V1 // default: INTERNAL1V1
        #define AD8318_TEMPERATURE_REF_VOLTAGE 1.1         // default: 1.1
    #elif defined(ARDUINO_AVR_LEONARDO)                    // mcu=atmega32u4
        #define AD8318_TEMPERATURE_REF_CONFIG  INTERNAL    // default: INTERNAL
        #define AD8318_TEMPERATURE_REF_VOLTAGE 2.56        // default: 2.56
    #elif defined(ENV_NATIVE)                              // unitest build
    #else
        #error "configuration missing for this MCU"
    #endif
#endif

// ----- section: auto power off

#define POWER_OFF_FEATURE // enable/disable shutdown feature
#if defined(POWER_OFF_FEATURE)
    #define POWER_OFF_SENSE_PIN               7   // input pin that triggers power off
    #define POWER_OFF_SENSE_PIN_TRIGGER_LEVEL LOW // input level that triggers power off
    #define POWER_OFF_SENSE_PIN_DELAY_MS      250 // milliseconds delay from pin being "released" to power off
    #define POWER_OFF_PIN                     8   // output pin to disrupt power via external hardware
    #define POWER_OFF_PIN_ACTIVE \
        LOW // LOW: pin goes from high to low on timeout
            // HIGH: pin goes low high to high on timeout

    // #define AUTO_POWER_OFF_FEATURE // enable/disable auto shutdown feature
    #if defined(AUTO_POWER_OFF_FEATURE)
        #define AUTO_POWER_OFF_SEC 300 // delay until power off if no interaction, comment out to disable feature
    #endif
#endif
