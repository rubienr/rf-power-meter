#include "OperatingState.h"

const char *emergencyTypeToStr(EmergencyType t)
{
    switch(t)
    {
    case EmergencyType::None:
        return "None";
    case EmergencyType::UnknownError:
        return "UnknownError";
    case EmergencyType::HaltOnUnrecoverableError:
        return "HaltOnUnrecoverableError";
    case EmergencyType::HaltOnUnrecoverableStorageError:
        return "HaltOnUnrecoverableStorageError";
    case EmergencyType::HaltOnUnrecoverableProbeError:
        return "HaltOnUnrecoverableProbeError";
#if defined(HAS_DISPLAY)
    case EmergencyType::HaltOnDisplayInitError:
        return "HaltOnDisplayInitError";
#endif // HAS_DISPLAY
#if defined(HAS_DATA_SINK_I2C)
    case EmergencyType::HaltOnDataSinkInitError:
        return "HaltOnDataSinkInitError";
#endif // HAS_DATA_SINK_I2C
    default:
        return "-";
    }
}
const char *operatingModeTypeToStr(OperatingModeType t)
{
    switch(t)
    {
    case OperatingModeType::EarlyInit:
        return "EarlyInit";
    case OperatingModeType::Setup:
        return "Setup";
#if defined(EEPROM_RESET_PIN_FEATURE)
    case OperatingModeType::ManualEepromReset:
        return "ManualEepromReset";
#endif
    case OperatingModeType::Operational:
        return "Operational";
    case OperatingModeType::AutoShutdown:
        return "AutoShutdown";
    case OperatingModeType::ShutdownRequested:
        return "ShutdownRequested";
    default:
        return "-";
    }
}

void OperatingState::switchMode(const OperatingModeType &next)
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

void OperatingState::setEmergency(const EmergencyType &e)
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
