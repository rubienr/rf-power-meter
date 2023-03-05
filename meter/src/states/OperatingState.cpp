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
    default:
        return "-";
    }
}
const char *operatingModeTypeToStr(OperatingModeType t)
{
    switch(t)
    {
    case OperatingModeType::Measure:
        return "Measure";
    case OperatingModeType::Idle:
        return "Idle";
    default:
        return "-";
    }
}
