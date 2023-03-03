#pragma once

template <typename T, T defaultValue, T minValue, T maxValue> struct ConstrainedNumericValue
{
public:
    ConstrainedNumericValue &operator=(const T &newValue)
    {
        m_value = constrain(newValue, minValue, maxValue);
        return *this;
    }

    const T &value() const { return m_value; }
    T &value() { return m_value; }
    void reset() { m_value = defaultValue; };

protected:
    T m_value{ constrain(defaultValue, minValue, maxValue) };

} __attribute__((__packed__));
