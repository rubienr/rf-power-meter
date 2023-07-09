#pragma once
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <string.h>
namespace avg
{

#include <stdio.h>
template <uint8_t entries_count, typename entry_t, typename sum_t, entry_t entry_t_min, entry_t entry_t_max> struct AverageT
{
    using entry_type = entry_t;
    using avg_type = sum_t;

    entry_t put(entry_t value)
    {
        if(0 == currentIndex)
        {
            currentMax = entry_t_min;
            currentMin = entry_t_max;
        }
        if(currentMax < value) currentMax = value;
        if(currentMin > value) currentMin = value;

        const entry_t last{values[currentIndex]};
        values[currentIndex] = value;
        totalSum -= last;
        totalSum += value;
        currentIndex = (currentIndex + 1) % maxValuesCount;

        return last;
    }

    void get(entry_t &average) const
    {
        average = static_cast<entry_t>(roundf(static_cast<float>(totalSum) / static_cast<float>(maxValuesCount)));
    }

    void get(entry_t &average, entry_t &min, entry_t &max) const
    {
        get(average);
        min = currentMin;
        max = currentMax;
    }

    void clear()
    {
        totalSum = 0;
        memset(values, 0, sizeof(values));
    }

    void setMaxCapacity(uint8_t valuesCount = entries_count - 1)
    {
        clear();
        maxValuesCount = valuesCount % (entries_count + 1);
    }

private:
    sum_t totalSum{0};
    entry_t values[entries_count]{0};
    uint8_t currentIndex{0};
    uint8_t maxValuesCount{entries_count};
    entry_t currentMax{entry_t_min};
    entry_t currentMin{entry_t_max};
};

} // namespace avg
