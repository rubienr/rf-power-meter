#include "firmware.h"
#include "../../lib/settings/log.hpp"

void (*reboot)(void) = 0;

void Firmware::setup()
{
    EarlyInitializer _{}; // TODO: this is 2nd attempt; 1st attempt does not initialize correctly
    operatingState.switchMode(OperatingModeType::Setup);

    { // initialize power off feature
#if defined(POWER_OFF_FEATURE)
        pinMode(POWER_OFF_SENSE_PIN, POWER_OFF_SENSE_PIN_TRIGGER_LEVEL == HIGH ? INPUT : INPUT_PULLUP);
        pinMode(POWER_OFF_PIN, OUTPUT);
        digitalWrite(POWER_OFF_PIN, POWER_OFF_PIN_ACTIVE == HIGH ? LOW : HIGH);
#endif
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

    { // initialize activity led feature
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, 0);
#if defined(ACTIVITY_LED)
        pinMode(ACTIVITY_LED, OUTPUT);
#endif
    }

    { // initialize display
        display.begin();
        display.setPowerSave(0);
        display.setFont(u8x8_font_chroma48medium8_r);
        display.clear();
    }

    { // initialize settings/EEPROM
        auto loadSettingsState = settings.storage.loadOrInit(settings.parameters);
        logLoadSettings(loadSettingsState);
        if(loadSettingsState.fatal_error) operatingState.setEmergency(EmergencyType::HaltOnUnrecoverableStorageError);
        else
        {
            Serial.println(F("{\n  \"settings\" : {"));
            LogSettings logger(Serial);
            logger.log(settings.parameters, 2);
            Serial.println(F("  }\n}"));
        }

#if defined(EEPROM_RESET_PIN_FEATURE)
        pinMode(EEPROM_RESET_PIN, EEPROM_RESET_PIN_TRIGGER_LEVEL == HIGH ? INPUT : INPUT_PULLUP);
#endif
    }

    { // initialize measure probe AD7887/AD8318
        pinMode(AD7887_CHIP_SELECT_PIN, OUTPUT);
        pinMode(AD7887_CLK_PIN, OUTPUT);
        pinMode(AD7887_DOUT_PIN, INPUT);
        pinMode(AD7887_DIN_PIN, OUTPUT);
#if defined(AD8318_TEMPERATURE_FEATURE)
        pinMode(AD8318_TEMPERATURE_PIN, INPUT);
        analogReference(AD8318_TEMPERATURE_REF_CONFIG);
#endif
    }

    operatingState.switchMode(OperatingModeType::Operational);
}

void Firmware::logLoadSettings(const StorageLoadResult &result)
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

void Firmware::logStoreSettings(const StorageStoreResult &result)
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
#endif
#if defined(AUTO_POWER_OFF_FEATURE)
    if(isAutoPowerOffTimeout() && operatingState.getMode() >= OperatingModeType::Operational)
        operatingState.switchMode(OperatingModeType::AutoShutdown);
#endif
#if defined(EEPROM_RESET_PIN_FEATURE)
    if(digitalRead(EEPROM_RESET_PIN) == EEPROM_RESET_PIN_TRIGGER_LEVEL && operatingState.getMode() >= OperatingModeType::Operational)
        operatingState.switchMode(OperatingModeType::ManualEepromReset);
#endif

    switch(operatingState.getMode())
    {
    case OperatingModeType::Operational:
        if(isSampleTimeout()) { doSample(); }
        if(isRenderTimeout()) { doRender(); }
#if defined(AD8318_TEMPERATURE_FEATURE)
        if(isTemperatureTimeout()) { doSampleTemperature(); }
#endif
        break;
    case OperatingModeType::ManualEepromReset:
        doResetAndReboot();

#if defined(POWER_OFF_FEATURE)
    case OperatingModeType::ShutdownRequested:
        Serial.print("#I Power off requested. Bye!");
        doPowerOff();
#endif

#if defined(AUTO_POWER_OFF_FEATURE)
    case OperatingModeType::AutoShutdown:
        Serial.print("#I Auto shutdown timeout. Bye!");
        doPowerOff();
#endif

    default:
        break;
    }
}

void Firmware::doSample()
{
#if defined(AD7887_SUBSEQUENT_READ_ERRORS)
    static uint8_t subsequentReadErrors{ 0 };
    if(!probe.device.readSample(probe.sampleRegister))
    {
        subsequentReadErrors++;
        if(subsequentReadErrors > AD7887_SUBSEQUENT_READ_ERRORS)
        {
            Serial.print(F("#F "));
            operatingState.setEmergency(EmergencyType::HaltOnUnrecoverableProbeError);
        }
        else { Serial.print(F("#W ")); }
        Serial.print(F("failed to read from probe ("));
        Serial.print(subsequentReadErrors);
        Serial.println(F(")"));
    }
    else { subsequentReadErrors = 0; }
#endif

#if defined(AD7887_SUBSEQUENT_ZERO_SAMPLES)
    static uint8_t subsequentZeroSamples{ 0 };
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
#endif

    Serial.print(F(R"({ "sample" : ")"));
    Serial.print(probe.sampleRegister.data);
    Serial.println(F("\" }"));
    timers.sampleMs = 0;
}

void Firmware::doRender()
{

#if !defined(ACTIVITY_LED)
    static uint8_t isOn{ 0 };
    isOn = isOn ? 0 : 1;
#else
    bool isOn{ toggleActivityLed() };
#endif
    display.setCursor(0, 0);
    display.print(isOn ? '+' : ' ');
    display.print(F("V="));
    display.println(probe.sampleRegister.data);

    display.display();

    timers.renderMs = 0;
}


#if defined(AD8318_TEMPERATURE_FEATURE)
void Firmware::doSampleTemperature()
{
    const int rawTemp10Bit{ analogRead(AD8318_TEMPERATURE_PIN) };
    const float miliVolt{ (rawTemp10Bit / (1024.0f - 1) * 1000 * static_cast<float>(AD8318_TEMPERATURE_REF_VOLTAGE)) };
    double tempKelvin = miliVolt / static_cast<float>(AD8318_TEMPERATURE_MILLI_VOLT_KELVIN);

    Serial.print(F(R"({ "temp" : { "mV" : ")"));
    Serial.print(miliVolt);
    Serial.print(F(R"(", "K" : ")"));
    Serial.print(tempKelvin);
    Serial.print(F(R"(", "C" : ")"));
    Serial.print(tempKelvin - 273.15);
    Serial.println(F("\" } } "));

    timers.temperatureMs = 0;
}
#endif

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

#if defined(POWER_OFF_FEATURE)
[[noreturn]] void Firmware::doPowerOff()
{
    while(digitalRead(POWER_OFF_SENSE_PIN) == POWER_OFF_SENSE_PIN_TRIGGER_LEVEL) {}
    delay(POWER_OFF_SENSE_PIN_DELAY_MS);

    digitalWrite(POWER_OFF_PIN, POWER_OFF_PIN_ACTIVE);

    for(;;) {}
}
#endif

[[noreturn]] void Firmware::doHalt()
{
    renderer.render();
    Serial.print(R"("{ EmergencyType" : ")");
    Serial.print(emergencyTypeToStr(operatingState.getEmergency()));
    Serial.println("\" }");
    for(;;) {}
}

#if defined(ACTIVITY_LED)
bool Firmware::toggleActivityLed()
{
    static bool isOn{ false };
    isOn = !isOn;
    digitalWrite(ACTIVITY_LED, isOn);
    return isOn;
}
#endif

bool Firmware::isSampleTimeout() { return timers.sampleMs > settings.parameters.sample.separation_ms.get(); }

bool Firmware::isRenderTimeout() { return timers.renderMs > settings.parameters.render.separation_ms.get(); }

#if defined(AD8318_TEMPERATURE_FEATURE)
bool Firmware::isTemperatureTimeout() { return timers.temperatureMs > settings.parameters.temperature.separation_ms.get(); }
#endif
#if defined(AUTO_POWER_OFF_FEATURE)
bool Firmware::isAutoPowerOffTimeout() { return timers.autoPowerOffSec > settings.parameters.device.autoPowerOffSeconds; }
void Firmware::resetAutoPowerOffTimeout() { timers.autoPowerOffSec = 0; }
#endif
