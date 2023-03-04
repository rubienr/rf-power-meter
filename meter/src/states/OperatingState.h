#pragma once
#include <inttypes.h>

enum class EmergencyType : uint8_t
{
    None = 0,
    HaltOnUnrecoverableError,
    HaltOnUnrecoverableStorageError,
    UnknownError,
};

const char *emergencyTypeToStr(EmergencyType t);

enum class OperatingMode : uint8_t
{
    Measure,
    Idle,
    Default = Measure
};

struct OperatingState
{
    EmergencyType emergency{ EmergencyType::None };
    OperatingMode mode{ OperatingMode::Default };
};
