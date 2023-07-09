#pragma once
#include "../lib/ad7887/AD7887.h"
#include "../lib/ad8138/AD8138.h"
#include "../lib/ad8138/progmemLoader.h"
#if defined(HAS_ENCODER)
    #include "../lib/pb_encoder/pb_encoder.h"
#endif // HAS_ENCODER
#include "../lib/average/Average.h"
#include "../lib/settings/Settings.h"
#include "../lib/storage/EepromStorageDevice.hpp"
#include "../lib/storage/SettingsStorage.hpp"
#include "elapsedMillis.h"
#include "states/OperatingState.h"
#include "ui/display/Display.h"
#include "ui/renderer/UiData.h"
#include "ui/renderer/UiRenderer.h"
#include <Arduino.h>
#include <HardwareSerial.h>

extern const float kValuesSets[6][4] PROGMEM;

using namespace settings;
using namespace storage;
using namespace ad8138;
using namespace ad7887;

void clkDigitalWrite(uint8_t digitalValue);
void chipSelectDigitalWrite(uint8_t digitalValue);
void dataDigitalWrite(uint8_t digitalValue);
int dataDigitalRead();

#if defined(HAS_ENCODER)
extern PbEncoder_Handle *pbEncoderHandle;
#endif // HAS_ENCODER

struct Resources
{
    OperatingState operatingState{};

    struct
    {
        Settings parameters{};
        SettingsStorage<EepromDevice<Settings>> storage{};
    } settings{};

#if defined(HAS_DISPLAY)
    UiData uiData{.uiContext = CurrentUiContext::Default,
                  .probe = {.rawSample12Bit = 0, .rawAverage12Bit = 0, .dbMilliW = 0, .watt = 0, .wattScale = UnitType::FEMTO},
                  .temperature = {0, 0, 0}};
#endif // HAS_DISPLAY

#if defined(HAS_DISPLAY)
    display_t display DISPLAY_INITIALIZER_LIST;
    UiRenderer renderer{uiData, operatingState, display, Serial};
#endif // HAS_DISPLAY

    struct
    {
        ControlRegister ctlRegister{.powerManagement0 = 1,
                                    .powerManagement1 = 0,
                                    .mustBeZero1 = 0,
                                    .channelSelect = 0,
                                    .singleDualChanelSelect = 0,
                                    .onChipReference = 1,
                                    .mustBeZero2 = 0,
                                    .dontCare = 0};
        AD7887 device{ctlRegister, chipSelectDigitalWrite, clkDigitalWrite, dataDigitalWrite, dataDigitalRead, delayMicroseconds};
        ad7887::SampleRegister sampleRegister{.raw12Bit = 0, .zero = 0};
        avg::Average32x16 sampleAverage;
        KValues3rdOrderFloat kValues{};
        AD8138Converter3rdOrder converter{kValues};

        struct
        {
            uint16_t rawSample10Bit{0};
            uint16_t volt_em4{0};
            uint16_t kelvin_em2{0};
            int16_t celsius_em2{0};
        } temperature;
    } probe{};

    struct
    {
        elapsedMillis sampleMs{0};
#if defined(HAS_DISPLAY)
        elapsedMillis renderMs{0};
#endif // HAS_DISPLAY
#if defined(HAS_DATA_SINK_I2C)
        elapsedMillis dataSinkMs{0};
#endif // HAS_DATA_SINK_I2C
#if defined(AD8318_TEMPERATURE_FEATURE)
        elapsedMillis temperatureMs{0};
#endif // AD8318_TEMPERATURE_FEATURE
#if defined(AUTO_POWER_OFF_FEATURE)
        elapsedSeconds autoPowerOffSec{0};
#endif // AUTO_POWER_OFF_FEATURE
    } timers{};

#if defined(HAS_ENCODER)
    PbEncoder_Handle encoderHandle{.counter = 0, .isPbPressed = 0};
#endif // HAS_ENCODER
};
