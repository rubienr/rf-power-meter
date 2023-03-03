#pragma once

#include <inttypes.h>

#include "../configuration.h"
#include "../util/constraints.h"
#include "ConstrainedNumericValue.hpp"

struct Version
{
    uint8_t major{ VERSION_MAJOR };
    uint8_t minor{ VERSION_MINOR };
    uint8_t patch{ VERSION_PATCH };
} __attribute__((__packed__));

struct DeviceInfo
{
    const Version version;
    uint32_t configWrites{ 0 };
} __attribute__((__packed__));

struct Sample
{
    ConstrainedNumericValue<uint16_t, SAMPLE_TIMER_MS_DEFAULT, SAMPLE_TIMER_MS_MIN, SAMPLE_TIMER_MS_MAX> sample_separation_ms{};
} __attribute__((__packed__));

struct Render
{
    ConstrainedNumericValue<uint16_t, RENDER_TIMER_MS_DEFAULT, RENDER_TIMER_MS_MIN, RENDER_TIMER_MS_MAX> render_separation_ms{};
} __attribute__((__packed__));

struct Settings
{
    DeviceInfo device;
    Render render;
    Sample sample;
} __attribute__((__packed__));
