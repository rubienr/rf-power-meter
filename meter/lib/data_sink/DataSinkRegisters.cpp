#include "DataSinkRegisters.h"

namespace datasink
{

size_t RegisterAddressIndex::operator++()
{
    auto tmp{address};
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

RegisterWhoAmI::RegisterWhoAmI() : asValue(registerConstantsToUnderlyingType(RegisterConstants::WhoAmI)) {}

} // namespace datasink
