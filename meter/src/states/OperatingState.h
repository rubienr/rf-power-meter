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

const char *emergencyTypeToStr(EmergencyType t);

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

const char *operatingModeTypeToStr(OperatingModeType t);

class OperatingState
{
public:
    void switchMode(const OperatingModeType &next);

    const OperatingModeType &getMode() { return mode; }

    void setEmergency(const EmergencyType &e);
    const EmergencyType &getEmergency() { return emergency; }

protected:
    EmergencyType emergency{ EmergencyType::None };
    OperatingModeType mode{ OperatingModeType::Default };
};
