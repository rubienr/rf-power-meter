#pragma once
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
    Measure,
    Idle,
    Default = Measure
};

const char *operatingModeTypeToStr(OperatingModeType t);

struct OperatingState
{
    EmergencyType emergency{ EmergencyType::None };
    OperatingModeType mode{ OperatingModeType::Default };
};
