#pragma once


template <typename T, T defaultValue, T minValue, T maxValue> struct ConstrainedNumericValue
{
    static_assert(defaultValue >= minValue && defaultValue <= maxValue && minValue <= maxValue, "assert(minValue <= defaultValue <= maxValue)");

public:
    explicit operator T() const { return value; }
    ConstrainedNumericValue &operator=(const ConstrainedNumericValue &) = default;
    ConstrainedNumericValue &operator=(const T &newValue)
    {
        value = ((newValue) < (minValue) ? (minValue) : ((newValue) > (maxValue) ? (maxValue) : (newValue)));
        return *this;
    };
    bool operator==(const ConstrainedNumericValue &other) const { return value == other.value; }
    T get() const { return value; }
    void reset() { value = defaultValue; };

protected:
    T value{ defaultValue };

} __attribute__((__packed__));
