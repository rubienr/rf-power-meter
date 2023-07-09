#pragma once
#include <EEPROM.h>

namespace storage
{

template <typename T> struct EepromDevice
{
    T &get(int idx, T &t) const { return EEPROM.get(idx, t); }
    const T &put(int idx, const T &t) { return EEPROM.put(idx, t); }
};

} // namespace storage
