#pragma once
#include <inttypes.h>

namespace i2c
{

struct AsBytes1
{
    uint8_t lowByte;
} __attribute__((__packed__));
static_assert(sizeof(AsBytes1) == 1);

struct AsBytes2
{
    uint8_t lowByte;
    uint8_t highByte;
} __attribute__((__packed__));
static_assert(sizeof(AsBytes2) == 2);

struct AsBytes4
{
    uint8_t lowByte;
    uint8_t highByte;
    uint8_t xLowByte;
    uint8_t xHighByte;
} __attribute__((__packed__));
static_assert(sizeof(AsBytes4) == 4);

union Data8
{
    AsBytes1 asBytes;
    uint8_t asUint8;
    int8_t asInt8;
} __attribute__((__packed__));
static_assert(sizeof(Data8) == 1);

union Data16
{
    AsBytes2 asBytes;
    uint16_t asUint16;
    int16_t asInt16;
} __attribute__((__packed__));
static_assert(sizeof(Data16) == 2);

union Data32
{
    AsBytes4 asBytes;
    float asFloat;
} __attribute__((__packed__));
static_assert(sizeof(Data32) == 4);

} // namespace i2c
