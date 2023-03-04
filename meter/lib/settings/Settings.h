#pragma once
#include "../configuration.h"
#include "ConstrainedNumericValue.hpp"
#include <inttypes.h>

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
} __attribute__((__packed__));

struct Sample
{
    Sample &operator=(const Sample &) = default;
    bool operator==(const Sample &other) const;

    ConstrainedNumericValue<uint16_t, SAMPLE_TIMER_MS_DEFAULT, SAMPLE_TIMER_MS_MIN, SAMPLE_TIMER_MS_MAX> separation_ms{};
} __attribute__((__packed__));

struct Render
{
    Render &operator=(const Render &) = default;
    bool operator==(const Render &other) const;
    ConstrainedNumericValue<uint16_t, RENDER_TIMER_MS_DEFAULT, RENDER_TIMER_MS_MIN, RENDER_TIMER_MS_MAX> separation_ms{};
} __attribute__((__packed__));

struct Settings
{
    const uint8_t *asPtr() const;
    Settings &operator=(const Settings &) = default;
    bool operator==(const Settings &other) const;
    uint32_t computeCrc() const;
    bool checkCrc() const;
    void updateCrc();

    DeviceInfo device;
    Render render;
    Sample sample;
    uint32_t crc{ 0 }; // CRC must be last field

} __attribute__((__packed__));
