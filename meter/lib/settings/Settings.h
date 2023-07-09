#pragma once
#if defined(POWER_METER)
    #include "../rfmeter_configuration.h"
    #include "ConstrainedNumericValue.hpp"
    #include <inttypes.h>

namespace settings
{

struct Version
{
    Version &operator=(const Version &) = default;
    bool operator==(const Version &other) const;

    uint8_t major{ VERSION_MAJOR };
    uint8_t minor{ VERSION_MINOR };
    uint8_t patch{ VERSION_PATCH };

} __attribute__((__packed__));

struct DeviceInfo
{
    DeviceInfo &operator=(const DeviceInfo &) = default;
    bool operator==(const DeviceInfo &other) const;

    Version version;
    uint32_t configWrites{ 0 };
    #if defined(AUTO_POWER_OFF_FEATURE)
    uint16_t autoPowerOffSeconds{ AUTO_POWER_OFF_SEC };
    #endif
} __attribute__((__packed__));

struct Sample
{
    Sample &operator=(const Sample &) = default;
    bool operator==(const Sample &other) const;

    ConstrainedNumericValue<uint16_t, SAMPLE_TIMER_MS_DEFAULT, SAMPLE_TIMER_MS_MIN, SAMPLE_TIMER_MS_MAX> separation_ms{};
} __attribute__((__packed__));

    #if defined(HAS_DISPLAY)
struct Render
{
    Render &operator=(const Render &) = default;
    bool operator==(const Render &other) const;
    ConstrainedNumericValue<uint16_t, RENDER_TIMER_MS_DEFAULT, RENDER_TIMER_MS_MIN, RENDER_TIMER_MS_MAX> separation_ms{};
} __attribute__((__packed__));
    #endif // HS_DISPLAY

    #if defined(HAS_DATA_SINK_I2C)
struct DataSink
{
    DataSink &operator=(const DataSink &) = default;
    bool operator==(const DataSink &other) const;
    ConstrainedNumericValue<uint16_t, DATA_SINK_TIMER_MS_DEFAULT, DATA_SINK_TIMER_MS_MIN, DATA_SINK_TIMER_MS_MAX> separation_ms{};
} __attribute__((__packed__));
    #endif // HAS_DATA_SINK_I2C

    #if defined(AD8318_TEMPERATURE_FEATURE)
struct Temperature
{
    Temperature &operator=(const Temperature &) = default;
    bool operator==(const Temperature &other) const;

    ConstrainedNumericValue<uint16_t, AD8318_TEMPERATURE_SAMPLE_MS, AD8318_TEMPERATURE_SAMPLE_MS_MIN, AD8318_TEMPERATURE_SAMPLE_MS_MAX> separation_ms{};
} __attribute__((__packed__));
    #endif

struct Settings
{
    const uint8_t *asPtr() const;
    Settings &operator=(const Settings &) = default;
    bool operator==(const Settings &other) const;
    uint32_t computeCrc() const;
    bool checkCrc() const;
    void updateCrc();

    DeviceInfo device;
    #if defined(HAS_DISPLAY)
    Render render;
    #endif // HAS_DISPLAY
    #if defined(HAS_DATA_SINK_I2C)
    DataSink dataSink;
    #endif // HAS_DATA_SINK_I2C
    Sample sample;
    #if defined(AD8318_TEMPERATURE_FEATURE)
    Temperature temperature;
    #endif
    uint32_t crc{ 0 }; // CRC must be last field

} __attribute__((__packed__));

} // namespace settings

#endif // POWER_METER
