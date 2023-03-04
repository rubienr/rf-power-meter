#pragma once

template <typename T, T defaultValue> struct DefaultNumericValue
{
public:
    explicit operator T() const { return value; }
    DefaultNumericValue &operator=(const DefaultNumericValue &) = default;
    DefaultNumericValue &operator=(const T &newValue)
    {
        value = newValue;
        return *this;
    }

    bool operator==(const DefaultNumericValue &other) const { return value == other.value; }
    T get() const { return value; }
    void reset() { value = defaultValue; };

protected:
    T value{ defaultValue };

} __attribute__((__packed__));
