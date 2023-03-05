#include "OperatingState.h"

const char *emergencyTypeToStr(EmergencyType t)
{
    switch(t)
    {
    case EmergencyType::None:
        return "None";
        break;
    case EmergencyType::UnknownError:
        return "UnknownError";
        break;
    case EmergencyType::HaltOnUnrecoverableError:
        return "HaltOnUnrecoverableError";
        break;
    case EmergencyType::HaltOnUnrecoverableStorageError:
        return "HaltOnUnrecoverableStorageError";
        break;
    default:
        return "-";
        break;
    }
}
