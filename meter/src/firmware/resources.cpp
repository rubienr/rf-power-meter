#include "resources.h"

void clkDigitalWrite(uint8_t digitalValue) { digitalWrite(AD7887_CLK_PIN, digitalValue); }
void chipSelectDigitalWrite(uint8_t digitalValue) { digitalWrite(AD7887_CHIP_SELECT_PIN, digitalValue); }
void dataDigitalWrite(uint8_t digitalValue) { digitalWrite(AD7887_DIN_PIN, digitalValue); }
int dataDigitalRead() { return digitalRead(AD7887_DOUT_PIN); }
