#pragma once

template <typename T> struct DummyStorageDevice
{
    T &get(int, T &t) const { return t; }
    const T &put(int, const T &t) { return t; }
};
