#pragma once
#include "../i2c/data.h"
#include <inttypes.h>
#include <string.h>

/// The data sink is where sampled data (raw, corrected, converted, averaged, ...) from the "meter" flows to.
/// The sink (buffer) is instantiated in the in the display's firmware (off-board display) but also referenced by the
/// meter (registers are instantiated on demand).
///
/// The buffer is organized linearly and addressed as common with I2C devices:
/// When bytes are written in batch (write transaction):
///  - 1st write-byte denotes the register pointer where to write next byte
///  - subsequent write-bytes are written at the register ptr. position; ptr. is incremented after each written byte
///  When bytes are read in batch (write-read transaction):
///  - 1st write-byte denotes the register pointer where to read next byte
///  - subsequent read requests are read from ptr. position; ptr. is incremented after read byte
///
///  Notes:
///  - multi byte registers (16 bit, 32 bit, ...) shall be written in one transaction to ensure a block data update
///  - scaled values (i.e. *10^-x etc.) have "_emx" appended
///  - example *10^-3 (e-3) _em3
///  - example *10^3 (e3) _ep3

namespace datasink
{
using namespace i2c;

enum class RegisterConstants : uint8_t
{
    WhoAmI = 0x44,
};

constexpr uint8_t registerConstantsToUnderlyingType(RegisterConstants c) { return static_cast<uint8_t>(c); }

union RegisterWhoAmI
{
    RegisterWhoAmI();
    AsBytes1 asBytes;
    uint8_t asValue;
} __attribute__((__packed__));
static_assert(sizeof(RegisterWhoAmI) == 1);

union RegisterPowerControl
{
    AsBytes1 asBytes;
    struct
    {
        uint8_t reboot : 1;      // reboot display controller request
        uint8_t initDisplay : 1; // re-initialize display request
        uint8_t _reserved : 6;
    } asValue;

} __attribute__((__packed__));
static_assert(sizeof(RegisterPowerControl) == 1);

union RegisterPowerSampleW
{
    AsBytes2 asBytes;
    uint16_t asValue_em4;
} __attribute__((__packed__));
static_assert(sizeof(RegisterPowerSampleW) == 2);

union RegisterPowerSampleDbMilliW
{
    AsBytes4 asBytes;
    float asValue;
} __attribute__((__packed__));
static_assert(sizeof(RegisterPowerSampleDbMilliW) == 4);

union RegisterTemperatureK
{
    AsBytes2 asBytes;
    uint16_t asValue_em2;
} __attribute__((__packed__));
static_assert(sizeof(RegisterTemperatureK) == 2);

union RegisterPowerSampleSeparationMs
{
    AsBytes1 asBytes;
    uint8_t asValue_ep1;
} __attribute__((__packed__));
static_assert(sizeof(RegisterPowerSampleSeparationMs) == 1);

union RegisterSampleConfig
{
    AsBytes1 asBytes;
    struct
    {
        uint8_t averageSamplesCount_1_to_32 : 5; // number of samples contributing to averaged value; 1 == no average
        uint8_t dbMwUnitType : 3;                // UnitType; SI unit of value w.r.t. DbmW
    } asValue;
} __attribute__((__packed__));
static_assert(sizeof(RegisterSampleConfig) == 1);

union RegisterTemperatureSampleSeparationMs
{
    AsBytes1 asByte;
    uint8_t asValue_ep1;
} __attribute__((__packed__));
static_assert(sizeof(RegisterTemperatureSampleSeparationMs) == 1);

union RegisterUi
{
    AsBytes1 asByte;
    struct
    {
        uint8_t activePage : 4;
        uint8_t activeItem : 4;
    } asValue;
} __attribute__((__packed__));
static_assert(sizeof(RegisterUi) == 1);

enum class RegisterAddress : uint8_t
{
    WhoAmI = 0,
    PowerControl = WhoAmI + sizeof(RegisterWhoAmI),
    PowerSampleDbMilliW = PowerControl + sizeof(RegisterPowerControl),
    PowerSampleW = PowerSampleDbMilliW + sizeof(RegisterPowerSampleDbMilliW),
    SampleConfig = PowerSampleW + sizeof(RegisterPowerSampleW),
    TemperatureK = SampleConfig + sizeof(RegisterSampleConfig),
    PowerSampleSeparationMs = TemperatureK + sizeof(RegisterTemperatureK),
    TemperatureSampleSeparationMs = PowerSampleSeparationMs + sizeof(RegisterPowerSampleSeparationMs),
    Ui = TemperatureSampleSeparationMs + sizeof(RegisterTemperatureSampleSeparationMs),
    LastValidAddress = Ui + sizeof(RegisterUi)
};

constexpr uint8_t registerAddressToUnderlyingType(RegisterAddress a) { return static_cast<uint8_t>(a); }

struct RegisterAddressIndex
{
    using UnderlyingType = size_t;

    UnderlyingType operator++();
    RegisterAddressIndex &operator=(UnderlyingType new_address);
    UnderlyingType operator*() const { return address; }

private:
    UnderlyingType address{0};
};

struct Registers
{
    // ID
    RegisterWhoAmI whoAmI;
    // sampling
    RegisterPowerControl powerControl{0};
    RegisterPowerSampleDbMilliW powerSampleDbMilliW{0};
    RegisterPowerSampleW powerSampleW{0};
    RegisterSampleConfig sampleConfig{0};
    RegisterTemperatureK temperatureK{0};
    RegisterPowerSampleSeparationMs powerSampleSeparationMs{0};
    RegisterTemperatureSampleSeparationMs temperatureSampleSeparationMs{0};
    // UI
    RegisterUi ui{};
} __attribute__((__packed__));
static_assert(sizeof(Registers) == static_cast<uint8_t>(RegisterAddress::LastValidAddress));

union MemoryMap
{
    Registers asRegisters{};
    uint8_t asBytes[sizeof(Registers)];
} __attribute__((__packed__));
static_assert(sizeof(MemoryMap) == static_cast<uint8_t>(RegisterAddress::LastValidAddress));

struct DataSink
{
    MemoryMap memory;
    RegisterAddressIndex address{};
};

} // namespace datasink
