#pragma once

#include <inttypes.h>

namespace i2c
{
int i2cMaster_ping(uint8_t i2cAddress);

int i2cMaster_clearBuffer();

int i2cMaster_read(uint8_t i2cAddress, uint8_t registerAddress, void *buffer, uint8_t howMany);

int i2cMaster_write(uint8_t i2cAddress, uint8_t registerAddress, const void *buffer, uint8_t howMany);

} // namespace i2c
