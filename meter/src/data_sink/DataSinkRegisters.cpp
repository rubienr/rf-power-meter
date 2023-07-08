#include "DataSinkRegisters.h"

namespace datasink
{
uint8_t registerAddressToUnderlyingType(RegisterAddress address) { return static_cast<uint8_t>(address); }

uint8_t registerConstantsToUnderlyingType(RegisterConstants c) { return static_cast<uint8_t>(c); }

size_t RegisterAddressIndex::operator++()
{
    auto tmp{ address };
    ++address;
    if(address > registerAddressToUnderlyingType(RegisterAddress::LastValidAddress)) address = 0;
    return tmp;
}

RegisterAddressIndex &RegisterAddressIndex::operator=(size_t new_address)
{
    address = new_address;
    if(address > registerAddressToUnderlyingType(RegisterAddress::LastValidAddress)) address = 0;
    return *this;
}

} // namespace datasink
