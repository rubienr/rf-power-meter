#pragma once
#include <inttypes.h>
#include <string.h>

namespace datasink
{
enum class RegisterConstants : uint8_t
{
    WhoAmI = 0x44,
};

uint8_t registerConstantsToUnderlyingType(RegisterConstants c);

struct AsBytes2
{
    uint8_t lowByte{ 0 };
    uint8_t highByte{ 0 };
} __attribute__((__packed__));

struct AsBytes4
{
    uint8_t lowByte{ 0 };
    uint8_t highByte{ 0 };
    uint8_t xLowByte{ 0 };
    uint8_t xHighByte{ 0 };
} __attribute__((__packed__));

union Data
{
    uint8_t asUint8;
    int8_t asInt8;
    uint16_t asUint16{ 0 };
    int16_t asInt16;
    float asFloat;
    AsBytes4 asBytes;
};

union RegisterWhoAmI
{
    uint8_t asByte;
    uint8_t asValue{ registerConstantsToUnderlyingType(RegisterConstants::WhoAmI) };
} __attribute__((__packed__));


union RegisterPowerControl
{
    uint8_t asByte;
    struct
    {
        uint8_t reboot : 1;
        uint8_t initDisplay : 1;
        uint8_t reserved : 6;
    } asValue;
} __attribute__((__packed__));

union RegisterPowerSampleDbmW
{
    AsBytes4 asBytes;
    float asValue;
} __attribute__((__packed__));

union RegisterTemperatureK
{
    AsBytes4 asBytes;
    float asValue{ 0.0f };
} __attribute__((__packed__));

union RegisterPowerSampleSeparationMs
{
    uint8_t asByte{ 0 };
    uint8_t asValue;
} __attribute__((__packed__));

union RegisterTemperatureSampleSeparationMs
{
    uint8_t asByte{ 0 };
    uint8_t asValue;
} __attribute__((__packed__));

enum class RegisterAddress : uint8_t
{
    WhoAmI = 0x00,
    PowerControl = WhoAmI + sizeof(RegisterWhoAmI),
    PowerSampleDbmW = PowerControl + sizeof(RegisterPowerControl),
    TemperatureK = PowerSampleDbmW + sizeof(RegisterPowerSampleDbmW),
    PowerSampleSeparationMS = TemperatureK + sizeof(RegisterTemperatureK),
    TemperatureSampleSeparationMS = PowerSampleSeparationMS + sizeof(RegisterPowerSampleSeparationMs),
    LastValidAddress = TemperatureSampleSeparationMS + sizeof(RegisterTemperatureSampleSeparationMs)
};

uint8_t registerAddressToUnderlyingType(RegisterAddress address);

struct RegisterAddressIndex
{
    using UnderlyingType = size_t;

    UnderlyingType operator++();
    RegisterAddressIndex &operator=(UnderlyingType new_address);
    UnderlyingType operator*() const { return address; }

private:
    UnderlyingType address{ 0 };
};

struct Registers
{
    RegisterWhoAmI whoAmI;
    RegisterPowerControl powerControl;
    RegisterPowerSampleDbmW powerSampleDb;
    RegisterTemperatureK temperatureK;
    RegisterPowerSampleSeparationMs powerSampleSeparationMs;
    RegisterTemperatureSampleSeparationMs temperatureSampleSeparationMs;
} __attribute__((__packed__));

union MemoryMap
{
    Registers asRegisters{};
    uint8_t asBytes[sizeof(Registers)];
} __attribute__((__packed__));

struct DataSink
{
    MemoryMap memory;
    RegisterAddressIndex address;
};

} // namespace datasink
