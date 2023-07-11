#include "master_transaction.h"

#include <Wire.h>
#include <errno.h>
namespace i2c
{

int i2cMaster_ping(uint8_t i2cAddress)
{
    Wire.beginTransmission(i2cAddress);
    if(1 != Wire.write(0)) return -EIO; // sets the register pointer to address 0x0
    return Wire.endTransmission(true);
}

int i2cMaster_clearBuffer()
{
    int cleared{-1};
    while(Wire.available())
    {
        Wire.read();
        cleared = 0;
    }
    return cleared;
}

int i2cMaster_read(uint8_t i2cAddress, uint8_t registerAddress, void *buffer, uint8_t howMany)
{
    if(nullptr == buffer || 0 == howMany) return -EINVAL;

    // write i2c address
    Wire.beginTransmission(i2cAddress);
    if(1 != Wire.write(registerAddress))
    {
        Wire.endTransmission(true);
        return -EIO;
    }
    int err{Wire.endTransmission(true)};
    if(err) return err;

    // request data
    if(howMany > Wire.requestFrom(i2cAddress, howMany)) return -EIO;
    if(!Wire.available()) return -EIO;
    for(uint8_t idx = 0; idx <= howMany && Wire.available(); idx++)
    {
        int c{Wire.read()};
        if(c < 0) return c;
        static_cast<uint8_t *>(buffer)[idx] = static_cast<uint8_t>(c);
    }

    return 0;
}

int i2cMaster_write(uint8_t i2cAddress, uint8_t registerAddress, const void *buffer, uint8_t howMany)
{
    Wire.beginTransmission(i2cAddress);
    if(1 != Wire.write(registerAddress)) return -EIO;
    if(howMany != static_cast<uint8_t>(Wire.write(static_cast<const uint8_t *>(buffer), howMany))) return -EIO;
    int err = Wire.endTransmission(true);
    if(err) return err;
    return 0;
}

} // namespace i2c
