#include "firmware.h"
#include "../lib/settings/LogSettings.hpp"
#if defined(HAS_DATA_SINK_I2C)
    #include "../lib/data_sink/DataSinkRegisters.h"
    #include "i2c/master_transaction.h"
    #include <Wire.h>
    #include <math.h>
#endif // HAS_DATA_SINK_I2C

#if defined(HAS_DATA_SINK_I2C)
using namespace datasink;
#endif // HAS_DATA_SINK_I2C

using KValuesLoader = KValues3rdOrderLoader<float>;

void Firmware::setup()
{
    operatingState.switchMode(OperatingModeType::Setup);

    { // initialize power off feature
#if defined(POWER_OFF_FEATURE)
        pinMode(POWER_OFF_SENSE_PIN, POWER_OFF_SENSE_PIN_TRIGGER_LEVEL == HIGH ? INPUT : INPUT_PULLUP);
        pinMode(POWER_OFF_PIN, OUTPUT);
        digitalWrite(POWER_OFF_PIN, POWER_OFF_PIN_ACTIVE == HIGH ? LOW : HIGH);
#endif // POWER_OFF_FEATURE
    }

    { // print device info
        Serial.print(F("#I RF Meter Version "));
        Serial.print(VERSION_MAJOR);
        Serial.print('.');
        Serial.print(VERSION_MINOR);
        Serial.print('.');
        Serial.print(VERSION_PATCH);
        Serial.print(F(" built "));
        Serial.print(__DATE__);
        Serial.print(F(" "));
        Serial.print(__TIME__);
        Serial.println();

        Serial.println(F("#I Notes:"));
        Serial.println(F("#I   - lines with leading '#' are human readable logs"));
        Serial.println(F("#I   - lines without leading '#' are slightly machine parseable"));
        Serial.println(F("#I   - D=DEBUG, I=INFO, E=ERROR, F=FATAL"));
    }

#if defined(HAS_ENCODER)
    pbEncoderHandle = &encoderHandle;
    PbEncoder_setup();
#endif // HAS_ENCODER

    if(!initActivityLed()) return;
#if defined(HAS_DISPLAY)
    if(!initDisplay()) return;
#endif // HAS_DISPLAY
#if defined(HAS_DATA_SINK_I2C)
    if(!initI2cDataSink()) return;
#endif // HAS_DATA_SINK_I2C
    if(!initEeprom()) return;
    if(!initPowerMeterProbe()) return;
    operatingState.switchMode(OperatingModeType::Operational);
}

void Firmware::logLoadSettings(const storage::LoadResult &result)
{
    if(result.loaded_crc_mismatch == 1) Serial.println(F("#W loaded settings from EEPROM: CRC mismatch"));
    if(result.loaded_version_mismatch == 1) Serial.println(F("#W loaded settings from EEPROM: version mismatch"));

    if(result.stored_defaults == 1) Serial.println(F("#I stored default settings to EEPROM"));
    if(result.loaded_defaults == 1) Serial.println(F("#I reloaded defaults from EEPROM"));

    if(result.loaded_crc_mismatch_after_storing_defaults == 1)
        Serial.println(F("#E failed to load settings from EEPROM: CRC mismatch"));
    if(result.loaded_version_mismatch_after_storing_defaults == 1)
        Serial.println(F("#E failed to load settings from EEPROM: version mismatch"));

    if(result.fatal_error == 1) Serial.println(F("#F fatal error while loading settings; EEPROM defect?"));
    else Serial.println(F("#I settings loaded successfully from EEPROM"));
}

void Firmware::logStoreSettings(const storage::StoreResult &result)
{
    if(result.stored == 1) Serial.println(F("#I settings successfully stored to EEPROM"));
    if(result.loaded_crc_mismatch_after_storing == 1) Serial.println(F("#F stored and reloaded settings have CRC error"));
    if(result.fatal_error == 1) Serial.println(F("#F fatal error storing loading settings; EEPROM defect?"));
}

void Firmware::process()
{
    switch(operatingState.getEmergency())
    {
    case EmergencyType::None:
        break;
    default:
        doHalt();
    }

#if defined(POWER_OFF_FEATURE)
    if(digitalRead(POWER_OFF_SENSE_PIN) == POWER_OFF_SENSE_PIN_TRIGGER_LEVEL && operatingState.getMode() >= OperatingModeType::Operational)
        operatingState.switchMode(OperatingModeType::ShutdownRequested);
#endif // POWER_OFF_FEATURE
#if defined(AUTO_POWER_OFF_FEATURE)
    if(isAutoPowerOffTimeout() && operatingState.getMode() >= OperatingModeType::Operational)
        operatingState.switchMode(OperatingModeType::AutoShutdown);
#endif // AUTO_POWER_OFF_FEATURE
#if defined(EEPROM_RESET_PIN_FEATURE)
    if(digitalRead(EEPROM_RESET_PIN) == EEPROM_RESET_PIN_TRIGGER_LEVEL && operatingState.getMode() >= OperatingModeType::Operational)
        operatingState.switchMode(OperatingModeType::ManualEepromReset);
#endif // EEPROM_RESET_PIN_FEATURE

    switch(operatingState.getMode())
    {
    case OperatingModeType::Operational:
        if(isPowerMeterSampleTimeout()) { doSamplePowerMeter(); }
#if defined(HAS_DISPLAY)
        if(isRenderTimeout()) { doRender(); }
#endif // HAS_DISPLAY
#if defined(HAS_DATA_SINK_I2C)
        if(isDataSinkTimeout()) { doSendCapturedData(); }
#endif
#if defined(AD8318_TEMPERATURE_FEATURE)
        if(isTemperatureSampleTimeout()) { doSampleTemperature(); }
#endif // AD8318_TEMPERATURE_FEATURE
        break;
#if defined(EEPROM_RESET_PIN_FEATURE)
    case OperatingModeType::ManualEepromReset:
        Serial.print("#I Reset and reboot requested. Bye!");
        doResetAndReboot();
#endif // EEPROM_RESET_PIN_FEATURE

#if defined(POWER_OFF_FEATURE)
    case OperatingModeType::ShutdownRequested:
        Serial.print("#I Power off requested. Bye!");
        doPowerOff();
#endif // POWER_OFF_FEATURE

#if defined(AUTO_POWER_OFF_FEATURE)
    case OperatingModeType::AutoShutdown:
        Serial.print("#I Auto shutdown timeout. Bye!");
        doPowerOff();
#endif // AUTO_POWER_OFF_FEATURE

    default:
        break;
    }
}

void Firmware::doSamplePowerMeter()
{
#if defined(HAS_ENCODER)
    if(encoderHandle.isPbPressed)
    {
        probe.sampleAverage.setCapacity(1);
        encoderHandle.isPbPressed = false;
    }
    if(encoderHandle.counter != 0)
    {
        if(encoderHandle.counter < 0) probe.sampleAverage.decreaseCapacity(abs(encoderHandle.counter));
        else if(encoderHandle.counter > 0) probe.sampleAverage.increaseCapacity(encoderHandle.counter);
        encoderHandle.counter = 0;
    }
#endif // HAS_ENCODER

#if !defined(AD7887_SUBSEQUENT_READ_ERRORS)
    probe.device.readSample(probe.sampleRegister);
    probe.sampleAverage.put(probe.sampleRegister.raw12Bit);
#else  // AD7887_SUBSEQUENT_READ_ERRORS
    bool success{probe.device.readSample(probe.sampleRegister)};
    static uint8_t subsequentReadErrors{0};

    if(!success)
    {
        subsequentReadErrors++;
        if(subsequentReadErrors > AD7887_SUBSEQUENT_READ_ERRORS)
        {
            Serial.print(F("#F "));
            operatingState.setEmergency(EmergencyType::HaltOnUnrecoverableProbeError);
            doRender();
        }
        else { Serial.print(F("#W ")); }
        Serial.print(F("failed to read from probe ("));
        Serial.print(subsequentReadErrors);
        Serial.println(F(")"));
    }
    else { subsequentReadErrors = 0; }
#endif // AD7887_SUBSEQUENT_READ_ERRORS

#if defined(AD7887_SUBSEQUENT_ZERO_SAMPLES)
    static uint8_t subsequentZeroSamples{0};
    if(probe.sampleRegister.data == 0)
    {
        subsequentZeroSamples++;
        if(subsequentZeroSamples > AD7887_SUBSEQUENT_ZERO_SAMPLES)
        {
            Serial.print(F("#F "));
            operatingState.setEmergency(EmergencyType::HaltOnUnrecoverableProbeError);
        }
        else { Serial.print(F("#W ")); }
        Serial.print(F("failed to read from probe: too many zero-samples ("));
        Serial.print(subsequentZeroSamples);
        Serial.println(F(")"));
    }
    else { subsequentZeroSamples = 0; }
#endif // AD7887_SUBSEQUENT_ZERO_SAMPLES
    avg::Average32x16::entry_type avgRaw;
    float dbMilliW;
    float watt;
    UnitType wattScale;
    probe.sampleAverage.get(avgRaw);
    probe.converter.convertDbMilliWatt(avgRaw, dbMilliW);
    probe.converter.convertWatt(dbMilliW, watt, wattScale);

#if defined(HAS_DISPLAY)
    uiData.power.dbMilliW = dbMilliW;
    uiData.power.watt_em4 = static_cast<uint8_t>(roundf(static_cast<float>(watt) * 1000.0f));
    uiData.power.wattScale = wattScale;
#endif // HAS_DISPLAY

    Serial.print(F(R"({ "sample" : { "raw" : ")"));
    Serial.print(probe.sampleRegister.raw12Bit);
    Serial.print(F(R"(", "raw~" : ")"));
    Serial.print(avgRaw);
    Serial.print(F(R"(", "dBmW" : ")"));
    Serial.print(dbMilliW);
    Serial.print(F(R"(", "W" : { "w" : ")"));
    Serial.print(watt);
    Serial.print(F(R"(", "siUnit" : ")"));
    Serial.print(si::unitTypeToChar(wattScale));
    Serial.println(F("\" } } }"));
    timers.sampleMs = 0;
}

#if defined(HAS_DATA_SINK_I2C)
void Firmware::doSendCapturedData()
{
    avg::Average32x16::entry_type avgRaw;
    float dbMilliW;
    float watt;
    UnitType wattScale;
    probe.sampleAverage.get(avgRaw);
    probe.converter.convertDbMilliWatt(avgRaw, dbMilliW);
    probe.converter.convertWatt(dbMilliW, watt, wattScale);


    RegisterPowerSampleDbMilliW powerDbMw{.asValue = dbMilliW};
    i2c::i2cMaster_write(DATA_SINK_I2C_ADDRESS, registerAddressToUnderlyingType(RegisterAddress::PowerSampleDbMilliW), &powerDbMw,
                         sizeof(powerDbMw));

    RegisterPowerSampleW powerWatt{.asValue_em4 = static_cast<uint16_t>(roundf(watt * 10.0f))};
    i2c::i2cMaster_write(DATA_SINK_I2C_ADDRESS, registerAddressToUnderlyingType(RegisterAddress::PowerSampleW), &powerWatt, sizeof(powerWatt));

    RegisterSampleConfig sampleCfg{.asValue = {.averageSamplesCount_1_to_32 = probe.sampleAverage.getCapacity(),
                                               .dbMwUnitType = unitTypeToUnderlyingType(wattScale)}};
    i2c::i2cMaster_write(DATA_SINK_I2C_ADDRESS, registerAddressToUnderlyingType(RegisterAddress::SampleConfig), &sampleCfg, sizeof(sampleCfg));

    RegisterTemperatureK tempKelvin{.asValue_em2 = probe.temperature.kelvin_em2};
    i2c::i2cMaster_write(DATA_SINK_I2C_ADDRESS, registerAddressToUnderlyingType(RegisterAddress::TemperatureK), &tempKelvin,
                         sizeof(tempKelvin));
}
#endif // HAS_DATA_SINK_I2C

void Firmware::doRender()
{
#if defined(HAS_DISPLAY)
    timers.renderMs = 0;
    renderer.render();
#endif // HAS_DISPLAY

#if defined(ACTIVITY_LED)
    toggleActivityLed();
#endif // ACTIVITY_LED
}


#if defined(AD8318_TEMPERATURE_FEATURE)
void Firmware::doSampleTemperature()
{
    probe.temperature.rawSample10Bit = analogRead(AD8318_TEMPERATURE_PIN);
    probe.temperature.volt_em4 =
    static_cast<uint16_t>(roundf(static_cast<float>(probe.temperature.rawSample10Bit) *
                                 static_cast<float>(AD8318_TEMPERATURE_REF_VOLTAGE) * (10000.0f / (1024.0f - 1.0f))));
    probe.temperature.kelvin_em2 = static_cast<uint16_t>(
    roundf((static_cast<float>(probe.temperature.volt_em4) * 10.0f) / static_cast<float>(AD8318_TEMPERATURE_MILLI_VOLT_KELVIN)));
    probe.temperature.celsius_em2 = static_cast<int16_t>(probe.temperature.kelvin_em2) - 27315;

    #if defined(HAS_DISPLAY)
    uiData.temperature.kelvin_em2 = probe.temperature.kelvin_em2;
    uiData.temperature.celsius_em2 = probe.temperature.celsius_em2;
    #endif // HAS_DISPLAY

    Serial.print(F(R"({ "temp" : { "raw" : ")"));
    Serial.print(probe.temperature.rawSample10Bit);
    Serial.print(F(R"(", "V" : ")"));
    Serial.print(probe.temperature.volt_em4);
    Serial.print(F(R"(e-4", "K" : ")"));
    Serial.print(probe.temperature.kelvin_em2);
    Serial.print(F(R"(e-2", "C" : ")"));
    Serial.print(probe.temperature.celsius_em2);
    Serial.println(F("e-2\" } } "));

    timers.temperatureMs = 0;
}
#endif // AD8318_TEMPERATURE_FEATURE

#if defined(EEPROM_RESET_PIN_FEATURE)
[[noreturn]] void Firmware::doResetAndReboot()
{
    while(digitalRead(EEPROM_RESET_PIN) == EEPROM_RESET_PIN_TRIGGER_LEVEL) {}
    delay(EEPROM_RESET_PIN_DELAY_MS);

    Settings defaultSettings;
    defaultSettings.device.configWrites = settings.parameters.device.configWrites;
    StorageStoreResult result = settings.storage.storeAndCheck(defaultSettings);
    logStoreSettings(result);
    Serial.flush();
    delay(100);

    reboot();
    for(;;) {}
}
#endif // EEPROM_RESET_PIN_FEATURE

#if defined(POWER_OFF_FEATURE)
[[noreturn]] void Firmware::doPowerOff()
{
    while(digitalRead(POWER_OFF_SENSE_PIN) == POWER_OFF_SENSE_PIN_TRIGGER_LEVEL) {}
    delay(POWER_OFF_SENSE_PIN_DELAY_MS);

    digitalWrite(POWER_OFF_PIN, POWER_OFF_PIN_ACTIVE);

    for(;;) {}
}
#endif // POWER_OFF_FEATURE

[[noreturn]] void Firmware::doHalt()
{
#if defined(HAS_DISPLAY)
    renderer.render();
#endif // HAS_DISPLAY
    Serial.print(R"("{ EmergencyType" : ")");
    Serial.print(emergencyTypeToStr(operatingState.getEmergency()));
    Serial.println("\" }");
    initActivityLed();
    for(;;)
    {
        toggleActivityLed();
        delay(100);
        toggleActivityLed();
        delay(100);
        toggleActivityLed();
        delay(100);
        toggleActivityLed();
        delay(100);
        toggleActivityLed();
        delay(500);
    }
}

bool Firmware::toggleActivityLed()
{
    static bool isOn{false};
    isOn = !isOn;
    digitalWrite(LED_BUILTIN, isOn);
    return isOn;
}

bool Firmware::isPowerMeterSampleTimeout() { return timers.sampleMs > settings.parameters.sample.separation_ms.get(); }

#if defined(HAS_DISPLAY)
bool Firmware::isRenderTimeout() { return timers.renderMs > settings.parameters.render.separation_ms.get(); }
#endif // HAS_DISPLAY

#if defined(HAS_DATA_SINK_I2C)
bool Firmware::isDataSinkTimeout() { return timers.dataSinkMs > settings.parameters.dataSink.separation_ms.get(); }
#endif // HAS_DATA_SINK_I2C

#if defined(AD8318_TEMPERATURE_FEATURE)
bool Firmware::isTemperatureSampleTimeout() { return timers.temperatureMs > settings.parameters.temperature.separation_ms.get(); }
#endif // AD8318_TEMPERATURE_FEATURE
#if defined(AUTO_POWER_OFF_FEATURE)
bool Firmware::isAutoPowerOffTimeout() { return timers.autoPowerOffSec > settings.parameters.device.autoPowerOffSeconds; }
void Firmware::resetAutoPowerOffTimeout() { timers.autoPowerOffSec = 0; }
#endif // AUTO_POWER_OFF_FEATURE

#if defined(HAS_DATA_SINK_I2C)
bool Firmware::initI2cDataSink()
{
    Wire.begin();
    Wire.setClock(DATA_SINK_I2C_SPEED);

    if(i2c::i2cMaster_ping(DATA_SINK_I2C_ADDRESS))
    {
        Serial.print(F("#E data-sink: failed to pint display address="));
        Serial.println(DATA_SINK_I2C_ADDRESS);
        operatingState.setEmergency(EmergencyType::HaltOnDataSinkInitError);
        return false;
    }

    RegisterWhoAmI current_id;
    int err = i2c::i2cMaster_read(DATA_SINK_I2C_ADDRESS, registerAddressToUnderlyingType(RegisterAddress::WhoAmI), &current_id,
                                  sizeof(current_id));
    if(err)
    {
        Serial.print(F("#E data-sink: failed to read 'Who Am I'-register from address="));
        Serial.println(DATA_SINK_I2C_ADDRESS);
        operatingState.setEmergency(EmergencyType::HaltOnDataSinkInitError);
        return false;
    }
    const uint8_t expected_id = registerConstantsToUnderlyingType(RegisterConstants::WhoAmI);
    if(current_id.asValue != expected_id)
    {
        Serial.print(F("#E data-sink: received 'Who Am I'-register device_id="));
        Serial.print(current_id.asValue);
        Serial.print(F(" but expected device_id="));
        Serial.println(registerConstantsToUnderlyingType(RegisterConstants::WhoAmI));
        operatingState.setEmergency(EmergencyType::HaltOnDataSinkInitError);
        return false;
    }

    Serial.print(F("#I data-sink: detected at I2C address="));
    Serial.println(HAS_DATA_SINK_I2C);
    Serial.print(F(" id="));
    Serial.println(current_id.asValue);

    RegisterPowerControl ctl{.asValue = {.reboot = 0, .initDisplay = 1, ._reserved = 0}};
    if(i2c::i2cMaster_write(DATA_SINK_I2C_ADDRESS, registerAddressToUnderlyingType(RegisterAddress::PowerControl), &ctl, sizeof(ctl)))
    {
        Serial.print(F("#E data-sink: failed to request 'init display' from device="));
        Serial.println(DATA_SINK_I2C_ADDRESS);
        operatingState.setEmergency(EmergencyType::HaltOnDataSinkInitError);
        return false;
    }

    return true;
}
#endif // HAS_DATA_SINK_I2C

bool Firmware::initPowerMeterProbe()
{
    // initialize measure probe: AD7887/AD8318
    pinMode(AD7887_CHIP_SELECT_PIN, OUTPUT);
    pinMode(AD7887_CLK_PIN, OUTPUT);
    pinMode(AD7887_DOUT_PIN, INPUT);
    pinMode(AD7887_DIN_PIN, OUTPUT);

#if defined(AD8318_TEMPERATURE_FEATURE)
    pinMode(AD8318_TEMPERATURE_PIN, INPUT);
    analogReference(AD8318_TEMPERATURE_REF_CONFIG);
#endif // AD8318_TEMPERATURE_FEATURE
    KValuesLoader::load(&kValuesSets[0][0], probe.kValues);
    char b[16]{0};
    double d;
    Serial.print(F(R"({ "kValues" : { "k0" : ")"));
    d = probe.kValues.k0;
    snprintf(b, 15, "%e", d);
    Serial.print(b);
    Serial.print(F(R"(", "k1" : ")"));
    d = probe.kValues.k1;
    snprintf(b, 15, "%e", d);
    Serial.print(b);
    Serial.print(F(R"(", "k2" : ")"));
    d = probe.kValues.k2;
    snprintf(b, 15, "%e", d);
    Serial.print(b);
    Serial.print(F(R"(", "k3" : ")"));
    d = probe.kValues.k3;
    snprintf(b, 15, "%e", d);
    Serial.print(b);
    Serial.println(F(" } }"));
    probe.converter.setCorrectionCoefficients(probe.kValues);

    return true;
}

bool Firmware::initEeprom()
{ // initialize settings/EEPROM
    auto loadSettingsState = settings.storage.loadOrInit(settings.parameters);
    logLoadSettings(loadSettingsState);
    if(loadSettingsState.fatal_error)
    {
        operatingState.setEmergency(EmergencyType::HaltOnUnrecoverableStorageError);
        return false;
    }
    else
    {
        Serial.println(F("{\n  \"settings\" : {"));
        LogSettings logger(Serial);
        logger.log(settings.parameters, 2);
        Serial.println(F("  }\n}"));
    }

#if defined(EEPROM_RESET_PIN_FEATURE)
    pinMode(EEPROM_RESET_PIN, EEPROM_RESET_PIN_TRIGGER_LEVEL == HIGH ? INPUT : INPUT_PULLUP);
#endif // EEPROM_RESET_PIN_FEATURE
    return true;
}

#if defined(HAS_DISPLAY)
bool Firmware::initDisplay()
{
    if(!displayInit(display))
    {
        operatingState.setEmergency(EmergencyType::HaltOnDisplayInitError);
        return false;
    }
    renderer.init();
    return true;
}
#endif // HAS_DISPLAY

bool Firmware::initActivityLed()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 0);
    return true;
}
