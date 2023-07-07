#include "firmware.h"
#include "data_sink/DataSinkRegisters.h"
#include <Wire.h>

using namespace datasink;

extern Firmware f;

void onDataSinkReceiveEvent(int count) { f.onWriteToSinkEvent(count); }
void onDataSinkRequestEvent() { f.onReadFromSinkEvent(); }

void (*reboot)(void) = 0;

void Firmware::setup()
{
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

    if(!initActivityLed()) return;
    if(!initI2cDataSource()) return;
    if(!initDisplay()) return;
    operatingState.switchMode(OperatingModeType::Operational);
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

    switch(operatingState.getMode())
    {
    case OperatingModeType::Operational:
        if(isRenderTimeout()) { doRender(); }
        break;

    default:
        break;
    }
}

void Firmware::doRender()
{
    timers.renderMs = 0;
#if defined(ACTIVITY_LED)
    toggleActivityLed();
#endif // ACTIVITY_LED
    uiData.probe.dbmW = sink.memory.asRegisters.powerSampleDb.asValue;
    uiData.temperature.kelvin = sink.memory.asRegisters.temperatureK.asValue;

    if(sink.memory.asRegisters.powerControl.asValue.initDisplay)
    {
        sink.memory.asRegisters.powerControl.asValue.initDisplay = 0;
        Serial.println("#I master requested 'init display'");
        renderer.init();
    }
    renderer.render();
}

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
    static bool isOn{ false };
    isOn = !isOn;
    digitalWrite(LED_BUILTIN, isOn);
    return isOn;
}

bool Firmware::isRenderTimeout() { return timers.renderMs > RENDER_TIMER_MS_DEFAULT; }

bool Firmware::initI2cDataSource()
{
    Wire.begin(DATA_SINK_I2C_ADDRESS);
    Wire.setClock(400000);
    Wire.onReceive(::onDataSinkReceiveEvent);
    Wire.onRequest(::onDataSinkRequestEvent);
    return true;
}

void Firmware::onReadFromSinkEvent()
{
    const auto address{ *sink.address };
    const auto value{ sink.memory.asBytes[*sink.address] };
    Serial.print("#I on request from addr=");
    Serial.print(static_cast<int16_t>(address));
    Serial.print(" value=");
    Serial.println(static_cast<int16_t>(value));
    Wire.write(value);
    sink.address++;
}

void Firmware::onWriteToSinkEvent(int count)
{
    // Serial.print("#I I2C write ");
    // Serial.print(count);
    // Serial.print(" byte(s)");

    if(count <= 0)
    {
        //  Serial.println();
        return;
    }

    // 1st write is register pointer
    sink.address = static_cast<RegisterAddressIndex::UnderlyingType>(Wire.read());
    // Serial.print(" addr=");
    // Serial.print(static_cast<int16_t>(*sink.address));
    //  subsequent writes to registers and auto-increments the register pointer
    while(Wire.available())
    {
        /*Serial.print(" [");
        Serial.print(static_cast<int16_t>(*sink.address));
        Serial.print("]=");*/
        sink.memory.asBytes[*sink.address] = Wire.read();
        // Serial.print(static_cast<int16_t>(sink.memory.asBytes[*sink.address]));
        sink.address++;
    }
    // Serial.println();
}

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

bool Firmware::initActivityLed()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 0);
    return true;
}
