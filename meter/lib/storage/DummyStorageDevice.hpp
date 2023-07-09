#pragma once

namespace storage
{

template <typename T> struct DummyDevice
{
    T &get(int, T &t) const { return t; }
    const T &put(int, const T &t) { return t; }
};

} // namespace storage
