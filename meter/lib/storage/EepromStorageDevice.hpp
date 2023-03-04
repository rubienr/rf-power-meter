#pragma once
#include <EEPROM.h>

template <typename T> struct EepromStorageDevice
{
    T &get(int idx, T &t) const { return EEPROM.get(idx, t); }
    const T &put(int idx, const T &t) { return EEPROM.put(idx, t); }
};
