#pragma once
#include "../lib/rfmeter_configuration.h"
#include <Arduino.h>
#include <inttypes.h>

enum class EmergencyType : uint8_t
{
    None = 0,
    HaltOnUnrecoverableError,
    HaltOnUnrecoverableStorageError,
    HaltOnUnrecoverableProbeError,
#if defined(HAS_DISPLAY)
    HaltOnDisplayInitError,
#endif // HAS_DISPLAY
#if defined(HAS_DATA_SINK_I2C)
    HaltOnDataSinkInitError,
#endif // HAS_DATA_SINK_I2C
    UnknownError,
};

enum class OperatingModeType : uint8_t
{
    EarlyInit,
    Setup,
#if defined(EEPROM_RESET_PIN_FEATURE)
    ManualEepromReset,
#endif
    Operational,
    AutoShutdown,
    ShutdownRequested,
    Default = EarlyInit
};

struct OperatingState
{
    void switchMode(const OperatingModeType &next);

    const OperatingModeType &getMode() { return mode; }

    void setEmergency(const EmergencyType &e);
    const EmergencyType &getEmergency() { return emergency; }

protected:
    EmergencyType emergency{EmergencyType::None};
    OperatingModeType mode{OperatingModeType::Default};
};


constexpr const char *emergencyTypeToStr(EmergencyType t)
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
constexpr const char *operatingStateToStr(OperatingModeType t)
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
