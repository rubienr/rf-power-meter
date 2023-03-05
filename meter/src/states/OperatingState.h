#pragma once
#include <Arduino.h>
#include <inttypes.h>

enum class EmergencyType : uint8_t
{
    None = 0,
    HaltOnUnrecoverableError,
    HaltOnUnrecoverableStorageError,
    HaltOnUnrecoverableProbeError,
    UnknownError,
};

const char *emergencyTypeToStr(EmergencyType t);

enum class OperatingModeType : uint8_t
{
    EarlyInit,
    Setup,
    ManualEepromReset,
    Operational,
    AutoShutdown,
    ShutdownRequested,
    Default = EarlyInit
};

const char *operatingModeTypeToStr(OperatingModeType t);

class OperatingState
{
public:
    void switchMode(const OperatingModeType &next)
    {
        Serial.print(F("#I "));
        Serial.print(operatingModeTypeToStr(mode));
        Serial.print(F(" -> "));
        mode = next;
        Serial.println(operatingModeTypeToStr(mode));

        Serial.print(F(R"({ "operatingMode" : ")"));
        Serial.print(operatingModeTypeToStr(mode));
        Serial.println(F("\" }"));
    }

    const OperatingModeType &getMode() { return mode; }

    void setEmergency(const EmergencyType &e)
    {
        Serial.print(F("#E "));
        Serial.print(emergencyTypeToStr(emergency));
        Serial.print(F(" -> "));
        emergency = e;
        Serial.println(emergencyTypeToStr(emergency));

        Serial.print(F(R"({ "emergency" : ")"));
        Serial.print(emergencyTypeToStr(emergency));
        Serial.println(F("\" }"));
    }
    const EmergencyType &getEmergency() { return emergency; }

protected:
    EmergencyType emergency{ EmergencyType::None };
    OperatingModeType mode{ OperatingModeType::Default };
};
