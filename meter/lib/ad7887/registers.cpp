#include "registers.h"

TransmissionData::TransmissionData() : asUint16(0) {}
TransmissionData::TransmissionData(const ControlRegister &ctlRegister) : asControlRegister(ctlRegister) {}

ReceptionData::ReceptionData() : asUint16(0) {}
