#include "si_unit_type.h"

const char siUnitTypeToStr(SiUnitType t)
{
    switch(t)
    {
    case SiUnitType::KILO:
        return 'k';
    case SiUnitType::TIMES_ONE:
        return ' ';
    case SiUnitType::MILLI:
        return 'm';
    case SiUnitType::MICRO:
        return 'u';
    case SiUnitType::NANO:
        return 'n';
    case SiUnitType::PICO:
        return 'p';
    case SiUnitType::FEMTO:
        return 'f';
    default:
        return '?';
    }
}
