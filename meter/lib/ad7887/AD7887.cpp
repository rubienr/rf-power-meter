#include "AD7887.h"


AD7887::AD7887(const ControlRegister &ctrlRegister,
               void (*clkDigitalWrite)(uint8_t),
               void (*chipSelectDigitalWrite)(uint8_t),
               void (*dataDigitalWrite)(uint8_t),
               int (*dataDigitalRead)(),
               void (*delayMicroSeconds)(unsigned int)) :
receptionData(),
transmissionData(ctrlRegister), clockWrite(clkDigitalWrite), chipSelectWrite(chipSelectDigitalWrite), dataWrite(dataDigitalWrite),
dataRead(dataDigitalRead), delayUs(delayMicroSeconds)
{
}


void AD7887::uint16Transaction()
{
    uint16_t currentDataBit = 1;

#if defined(AD7887_T2_DELAY_US)
    delayUs(AD7887_T2_DELAY_US);
#endif
    chipSelectWrite(0);
    while(currentDataBit)
    {
        // 1. write data
        // 2. toggle clock
        // 3. read data
        dataWrite(currentDataBit & transmissionData.asUint16);

        clockWrite(0);
#if defined(AD7887_T7_DELAY_US)
        delayUs(AD7887_T7_DELAY_US);
#endif
        clockWrite(1);
#if defined(AD7887_T6_DELAY_US)
        delayUs(AD7887_T6_DELAY_US);
#endif
        receptionData.asUint16 += dataRead();
        receptionData.asUint16 <<= 1;

        currentDataBit <<= 1;
    }
    chipSelectWrite(1);
}

bool AD7887::readSample(SampleRegister &sample)
{
    if(receptionData.asSampleRegister.zero != 0) return false;
    sample = receptionData.asSampleRegister;
    return true;
}


void AD7887::updateControlRegister(const ControlRegister &newFlags) { transmissionData.asControlRegister = newFlags; }
