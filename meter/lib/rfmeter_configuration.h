#pragma once

// ----- section: version

#if defined(POWER_METER)
    #define VERSION_MAJOR 0
    #define VERSION_MINOR 0
    #define VERSION_PATCH 2
#elif defined(OFFBOARD_DISPLAY)
    #define VERSION_MAJOR 0
    #define VERSION_MINOR 0
    #define VERSION_PATCH 1
#elif defined(ENV_NATIVE)
    #define VERSION_MAJOR 0
    #define VERSION_MINOR 0
    #define VERSION_PATCH 0
else
    #error
#endif

// ----- section: probe sampling

#if defined(POWER_METER)
    #define SAMPLE_TIMER_MS_DEFAULT 50
    #define SAMPLE_TIMER_MS_MAX     1000
    #define SAMPLE_TIMER_MS_MIN     30
#endif // POWER_METER

// ----- section: serial

#if !defined(SERIAL_LOG_BAUD_RATE)
    #define SERIAL_LOG_BAUD_RATE 115200 // 115200, 230400, 460800
#endif                                  // SERIAL_BAUD_RATE
#define SERIAL_LOG_CONN_MODE SERIAL_8N1

// ----- section: push-button encoder (2 encoder signals + 1 push button signals)

#define ENCODER_FEATURE
#if defined(ENCODER_FEATURE)
    #define HAS_ENCODER
    #if defined(ARDUINO_AVR_UNO)          // mcu=atmega328p
        #define ENCODER_PUSHBUTTON_PIN 11 // interrupt capable pin
        #define ENCODER_SIGNAL_A_PIN   10 // interrupt capable pin
        #define ENCODER_SIGNAL_B_PIN   9  // digital pin
    #elif defined(ARDUINO_AVR_MEGA2560)   // mcu=atmega2560
        #define ENCODER_PUSHBUTTON_PIN 13 // interrupt capable pin
        #define ENCODER_SIGNAL_A_PIN   12 // interrupt capable pin
        #define ENCODER_SIGNAL_B_PIN   11 // digital pin
    #elif defined(ARDUINO_AVR_LEONARDO)   // mcu=atmega32u4
        #define ENCODER_PUSHBUTTON_PIN 11 // interrupt capable pin
        #define ENCODER_SIGNAL_A_PIN   10 // interrupt capable pin
        #define ENCODER_SIGNAL_B_PIN   9  // digital pin
    #elif defined(ARDUINO_AVR_MICRO)      // mcu=atmega32u4
        #define ENCODER_PUSHBUTTON_PIN 2  // interrupt capable pin
        #define ENCODER_SIGNAL_A_PIN   3  // interrupt capable pin
        #define ENCODER_SIGNAL_B_PIN   4  // digital pin
    #elif defined(ARDUINO_AVR_NANO)       // mcu=atmega328p or mcu=atmega328pb
        #define ENCODER_PUSHBUTTON_PIN 2  // interrupt capable pin
        #define ENCODER_SIGNAL_A_PIN   3  // interrupt capable pin
        #define ENCODER_SIGNAL_B_PIN   4  // digital pin
    #elif defined(ENV_NATIVE)
    #else
        #error
    #endif
#endif // HAS_ENCODER

// ----- section: display (on-board: connected to meter; off-board: dedicated controller)

#if defined(POWER_METER) // on-board display: not recommended
    #if !defined(DISPLAY_FEATURE)
    // #define DISPLAY_FEATURE     // enable/disable display functionality
    #endif
#endif // POWER_METER

#if defined(DISPLAY_FEATURE) || defined(OFFBOARD_DISPLAY)
    #define HAS_DISPLAY
    #define RENDER_TIMER_MS_DEFAULT 250
    #define RENDER_TIMER_MS_MAX     1000 // config storage: max
    #define RENDER_TIMER_MS_MIN     100  // config storage: min

    #if defined(DISPLAY_I2C)
        #define OLED_RST U8X8_PIN_NONE
        #define OLED_SCL SCL
        #define OLED_SDA SDA
        #define DISPLAY_INITIALIZER_LIST     \
            {                                \
                OLED_RST, OLED_SCL, OLED_SDA \
            }
    #elif defined(DISPLAY_PARALLEL_8BIT)
        #define TFT_EXPECTED_ID 0x8230
        #define TFT_LCD_RESET   A4
        #define TFT_LCD_CS      A3
        #define TFT_LCD_CD      A2
        #define TFT_LCD_WR      A1
        #define TFT_LCD_RD      A0
        #define DISPLAY_INITIALIZER_LIST                                      \
            {                                                                 \
                TFT_LCD_CS, TFT_LCD_CD, TFT_LCD_WR, TFT_LCD_RD, TFT_LCD_RESET \
            }
    #else
        #error
    #endif
#endif // DISPLAY_FEATURE

// ----- data consumer (i.e. off-board display): I2C address and bus pins

#if defined(OFFBOARD_DISPLAY) || (defined(POWER_METER) && !defined(DISPLAY_FEATURE))
    #define HAS_DATA_SINK_I2C
    #define DATA_SINK_TIMER_MS_DEFAULT 50
    #define DATA_SINK_TIMER_MS_MAX     1000
    #define DATA_SINK_TIMER_MS_MIN     30

    #define DATA_SINK_I2C_ADDRESS      ((uint8_t)0x18U) // address of data consumer
    #if defined(ARDUINO_AVR_UNO)                        // mcu=atmega328p
        #define DATA_SINK_I2C_SCL PIN_WIRE_SCL
        #define DATA_SINK_I2C_SDA PIN_WIRE_SDA
    #elif defined(ARDUINO_AVR_MEGA2560) // mcu=atmega2560
        #define DATA_SINK_I2C_SCL PIN_WIRE_SCL
        #define DATA_SINK_I2C_SDA PIN_WIRE_SDA
    #elif defined(ARDUINO_AVR_LEONARDO) // mcu=atmega32u4
        #define DATA_SINK_I2C_SCL PIN_WIRE_SCL
        #define DATA_SINK_I2C_SDA PIN_WIRE_SDA
    #elif defined(ARDUINO_AVR_MICRO) // mcu=atmega32u4
        #define DATA_SINK_I2C_SCL PIN_WIRE_SCL
        #define DATA_SINK_I2C_SDA PIN_WIRE_SDA
    #elif defined(ARDUINO_AVR_NANO) // mcu=atmega328p or mcu=atmega328pb
        #define DATA_SINK_I2C_SCL PIN_WIRE_SCL
        #define DATA_SINK_I2C_SDA PIN_WIRE_SDA
    #elif defined(ENV_NATIVE)
    #else
        #error
    #endif
#endif // OFFBOARD_DISPLAY || POWER_METER && !DISPLAY_FEATURE

// ----- section: periphery

#define ACTIVITY_LED // enables LED_BUILTIN blinking on each render loop; disable in production

// ----- section: EEPROM settings storage

#if defined(POWER_METER)
    #define EEPROM_START_ADDRESS 0x0 // move the start address if EEPROM is broken to avoid damaged segments
    // #define EEPROM_RESET_PIN_FEATURE     // enable/disable manual EEPROM reset feature
    #if defined(EEPROM_RESET_PIN_FEATURE)
        #if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_LEONARDO)
            #define EEPROM_RESET_PIN 6   // input pin that triggers reset
        #elif defined(ARDUINO_AVR_MEGA2560)
            #define EEPROM_RESET_PIN PA0 // input pin that triggers reset
        #else
            #error
        #endif
        #define EEPROM_RESET_PIN_TRIGGER_LEVEL LOW // input level that triggers reset
        #define EEPROM_RESET_PIN_DELAY_MS      250 // milliseconds delay from pin being "released" to power off
    #endif                                         // EEPROM_RESET_PIN_FEATURE
#endif                                             // POWER_METER

// ----- section: data acquisition and conversion

#if defined(POWER_METER)

// ----- section: AD7887 ( 12-bit micro power ADC)

    #if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_LEONARDO)
        #define AD7887_CHIP_SELECT_PIN 10 // CS   - J2, pin 2; chip select to AD7887
        #define AD7887_CLK_PIN         9  // SCLK - J2, pin 1; clock to AD7887
        #define AD7887_DOUT_PIN        12 // DOUT - J2, pin 3; data from AD7887
        #define AD7887_DIN_PIN         11 // DIN  - J2, pin 5; data to AD7887
    #elif defined(ARDUINO_AVR_MEGA2560)
        #define AD7887_CHIP_SELECT_PIN 46 // CS   - J2, pin 2; chip select to AD7887
        #define AD7887_CLK_PIN         48 // SCLK - J2, pin 1; clock to AD7887
        #define AD7887_DOUT_PIN        42 // DOUT - J2, pin 3; data from AD7887
        #define AD7887_DIN_PIN         44 // DIN  - J2, pin 5; data to AD7887
    #elif defined(ARDUINO_AVR_MICRO)
        #define AD7887_CHIP_SELECT_PIN 28 // CS   - J2, pin 2; chip select to AD7887
        #define AD7887_CLK_PIN         1  // SCLK - J2, pin 1; clock to AD7887
        #define AD7887_DOUT_PIN        29 // DOUT - J2, pin 3; data from AD7887
        #define AD7887_DIN_PIN         30 // DIN  - J2, pin 5; data to AD7887
    #elif defined(ARDUINO_AVR_NANO)
        #define AD7887_CHIP_SELECT_PIN 8  // CS   - J2, pin 2; chip select to AD7887
        #define AD7887_CLK_PIN         7  // SCLK - J2, pin 1; clock to AD7887
        #define AD7887_DOUT_PIN        9  // DOUT - J2, pin 3; data from AD7887
        #define AD7887_DIN_PIN         10 // DIN  - J2, pin 5; data to AD7887
    #elif defined(ENV_NATIVE)
    #else
        #error
    #endif


    // The maximum clock frequency is f_sclk=2MHz, thus a clock-signal hold of t_sclk=500ns is the least appropriate delay.
    // Note:
    // - the recommended values below are meant vor VDD 4.75V to 5.25V
    // - wait for nanoseconds is not possible, thus most delays end in 1µS
    #define AD7887_T2_DELAY_US 1 // max 30ns; CS until DOUT three-state disabled; comment out to disable
    #define AD7887_T6_DELAY_US 1 // min 0.4*t_sclk; clock high pulse width; comment out to disable
    #define AD7887_T7_DELAY_US 1 // min 0.4*t_sclk; clock low pulse width; comment out to disable

// #define AD7887_SUBSEQUENT_READ_ERRORS 3 // maximum allowed read errors before op-mode switches to error mode; comment to disable
// #define AD7887_SUBSEQUENT_ZERO_SAMPLES 3 // maximum allowed 0x000-valued samples before op-mode switches to error mode; comment to disable

// ----- section: AD8318 (1 MHz to 8 GHz, 70 dB Logarithmic Detector/Controller)

    #define AD8318_TEMPERATURE_FEATURE
    #if defined(AD8318_TEMPERATURE_FEATURE)
        #define AD8318_TEMPERATURE_SAMPLE_MS     1000 // separation in between temperature measurements
        #define AD8318_TEMPERATURE_SAMPLE_MS_MIN 500  // min configurable separation
        #define AD8318_TEMPERATURE_SAMPLE_MS_MAX 5000 // max configurable separation
        #if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_LEONARDO)
            #define AD8318_TEMPERATURE_PIN A0         // TEMP - J2, pin 4; analog temperature out from AD8318
        #elif defined(ARDUINO_AVR_MEGA2560)
            #define AD8318_TEMPERATURE_PIN A15        // TEMP - J2, pin 4; analog temperature out from AD8318
        #elif defined(ARDUINO_AVR_MICRO)
            #define AD8318_TEMPERATURE_PIN A5         // TEMP - J2, pin 4; analog temperature out from AD8318
        #elif defined(ARDUINO_AVR_NANO)
            #define AD8318_TEMPERATURE_PIN A0         // TEMP - J2, pin 4; analog temperature out from AD8318
        #elif defined(ENV_NATIVE)
        #else
            #error
        #endif
        #define AD8318_TEMPERATURE_MILLI_VOLT_KELVIN 2.0 // 2mV/°K; about 600mV at 27°C (273.15°K+27°K)

        // see:
        // -`analogReference()`
        // - https://reference.arduino.cc/reference/en/language/functions/analog-io/analogreference/
        #if defined(ARDUINO_AVR_UNO)                        // mcu=atmega328p
            #define AD8318_TEMPERATURE_REF_CONFIG  EXTERNAL // default: INTERNAL; the 1.1V ref. may not be as accurate as expected
            #define AD8318_TEMPERATURE_REF_VOLTAGE 3.3      // default: 1.1
        #elif defined(ARDUINO_AVR_MEGA2560)                 // mcu=atmega2560
            #define AD8318_TEMPERATURE_REF_CONFIG  INTERNAL1V1 // default: INTERNAL1V1
            #define AD8318_TEMPERATURE_REF_VOLTAGE 1.1         // default: 1.1
        #elif defined(ARDUINO_AVR_LEONARDO)                    // mcu=atmega32u4
            #define AD8318_TEMPERATURE_REF_CONFIG  INTERNAL    // default: INTERNAL
            #define AD8318_TEMPERATURE_REF_VOLTAGE 2.56        // default: 2.56
        #elif defined(ARDUINO_AVR_MICRO)                       // mcu=atmega32u4
            #define AD8318_TEMPERATURE_REF_CONFIG  INTERNAL    // default: INTERNAL
            #define AD8318_TEMPERATURE_REF_VOLTAGE 1.1         // default: 1.1
        #elif defined(ARDUINO_AVR_NANO)                        // mcu=atmega328p or mcu=atmega328pb
            #define AD8318_TEMPERATURE_REF_CONFIG  INTERNAL    // default: INTERNAL
            #define AD8318_TEMPERATURE_REF_VOLTAGE 1.1         // default: 1.1
        #elif defined(ENV_NATIVE)                              // unit test build
        #else
            #error
        #endif
    #endif                                         // AD8318_TEMPERATURE_FEATURE

    #define AD8318_CALIBRATION_FREQUENCY_MIN     0 // min index in AD8318_CALIBRATION_FACTORS
    #define AD8318_CALIBRATION_FREQUENCY_MAX     5 // max index in AD8318_CALIBRATION_FACTORS
    #define AD8318_CALIBRATION_FREQUENCY_DEFAULT 1 // default index in AD8318_CALIBRATION_FACTORS

    // frequencies that have calibration sets in AD8318_CALIBRATION_FACTORS
    #define AD8318_CALIBRATION_FREQUENCY_MHZ \
        {                                    \
            50, 145, 433, 1000, 2000, 3000   \
        }

    // Calibration factors layout - AD8318_CALIBRATION_FACTORS
    // { { k0, k1, k2, k3 },  for AD8318_CALIBRATION_FREQUENCY_MHZ[0]
    //   { k0, k1, k2, k3 },  for AD8318_CALIBRATION_FREQUENCY_MHZ[1]
    //   { k0, k1, k2, k3 },  for AD8318_CALIBRATION_FREQUENCY_MHZ[2]
    //   { k0, k1, k2, k3 },  for AD8318_CALIBRATION_FREQUENCY_MHZ[3]
    //   { k0, k1, k2, k3 },  for AD8318_CALIBRATION_FREQUENCY_MHZ[4]
    //   { k0, k1, k2, k3 } } for AD8318_CALIBRATION_FREQUENCY_MHZ[5]
    //
    // clang-format off
    #define AD8318_CALIBRATION_FACTORS \
        {                              \
            { 2.80342581E+01, -3.40466467E-02, 5.36059447E-06, -9.64826574E-10 }, \
            { 2.81602896E+01, -3.43993780E-02, 5.36926411E-06, -9.35643168E-10 }, \
            { 2.73776126E+01, -3.35128770E-02, 4.76795974E-06, -8.13901646E-10 }, \
            { 2.63845320E+01, -3.25466666E-02, 4.19419955E-06, -7.11984824E-10 }, \
            { 2.63845320E+01, -3.25466666E-02, 4.19419955E-06, -7.11984824E-10 }, \
            { 2.63845320E+01, -3.25466666E-02, 4.19419955E-06, -7.11984824E-10 }  \
        }
// clang-format on
#endif // POWER_METER


// ----- section: power saving

#if defined(POWER_METER)
    // #define POWER_OFF_FEATURE                         // enable/disable shutdown feature
    #if defined(POWER_OFF_FEATURE)
        // ----- section: power off
        #define POWER_OFF_SENSE_PIN               7   // input pin that triggers power off
        #define POWER_OFF_SENSE_PIN_TRIGGER_LEVEL LOW // input level that triggers power off
        #define POWER_OFF_SENSE_PIN_DELAY_MS      250 // milliseconds delay from pin being "released" to power off
        #define POWER_OFF_PIN                     8   // output pin to disrupt power via external hardware
        #define POWER_OFF_PIN_ACTIVE \
            LOW                                       // LOW: pin goes from high to low on timeout
                                                      // HIGH: pin goes low high to high on timeout

        // ----- section: auto power off
        // #define AUTO_POWER_OFF_FEATURE // enable/disable auto shutdown feature
        #if defined(AUTO_POWER_OFF_FEATURE)
            #define AUTO_POWER_OFF_SEC 300 // delay until power off if no interaction, comment out to disable feature
        #endif
    #endif                                 // POWER_OFF_FEATURE

#endif                                     // POWER_METER


// ----- section: sanity checks

#if defined(POWER_METER) && defined(OFFBOARD_DISPLAY)
    #error define either POWER_METER or OFFBOARD_DISPLAY but not both
#endif

#if defined(DISPLAY_I2C) && defined(DISPLAY_PARALLEL_8BIT)
    #error define either DISPLAY_I2C or DISPLAY_PARALLEL_8BIT but not both
#endif
