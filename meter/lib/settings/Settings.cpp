#include "Settings.h"
#if defined(POWER_METER)
    #include "../crc/crc.h"
namespace settings
{

const uint8_t *Settings::asPtr() const { return reinterpret_cast<const uint8_t *>(this); }

bool Version::operator==(const Version &other) const
{
    return major == other.major && minor == other.minor && patch == other.patch;
}

bool DeviceInfo::operator==(const DeviceInfo &other) const
{
    return version == other.version && configWrites == other.configWrites;
}

bool Sample::operator==(const Sample &other) const { return separation_ms == other.separation_ms; }

    #if defined(HAS_DISPLAY)
bool Render::operator==(const Render &other) const { return separation_ms == other.separation_ms; }
    #endif // HAS_DISPLAY

    #if defined(AD8318_TEMPERATURE_FEATURE)
bool Temperature::operator==(const Temperature &other) const { return separation_ms == other.separation_ms; }
    #endif

bool Settings::operator==(const Settings &other) const
{
    return device == other.device &&
    #if defined(HAS_DISPLAY)
           render == other.render &&
    #endif // HAS_DISPLAY
           sample == other.sample && crc == other.crc;
}

uint32_t Settings::computeCrc() const { return ::Crc_compute(asPtr(), sizeof(Settings) - sizeof(crc)); }

bool Settings::checkCrc() const { return crc == computeCrc(); }

void Settings::updateCrc() { crc = computeCrc(); }

} // namespace settings
#endif // POWER_METER
