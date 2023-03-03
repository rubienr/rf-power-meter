#pragma once

#include <inttypes.h>

enum class EmergencyType : uint8_t
{
    HaltOnUnrecoverableError,
    UnknownError,
    None,
};

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
