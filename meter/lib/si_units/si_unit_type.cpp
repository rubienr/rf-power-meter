#include "si_unit_type.h"

namespace si
{

const char unitTypeToStr(UnitType t)
{
    switch(t)
    {
    case UnitType::KILO:
        return 'k';
    case UnitType::TIMES_ONE:
        return ' ';
    case UnitType::MILLI:
        return 'm';
    case UnitType::MICRO:
        return 'u';
    case UnitType::NANO:
        return 'n';
    case UnitType::PICO:
        return 'p';
    case UnitType::FEMTO:
        return 'f';
    default:
        return '?';
    }
}

} // namespace si
