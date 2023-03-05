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
    case OperatingModeType::EarlyInit:
        return "EarlyInit";
    case OperatingModeType::Setup:
        return "Setup";
    case OperatingModeType::ManualEepromReset:
        return "ManualEepromReset";
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
