#include "OperatingState.h"

void OperatingState::switchMode(const OperatingModeType &next)
{
    Serial.print(F("#I "));
    Serial.print(operatingStateToStr(mode));
    Serial.print(F(" -> "));
    mode = next;
    Serial.println(operatingStateToStr(mode));

    Serial.print(F(R"({ "operatingMode" : ")"));
    Serial.print(operatingStateToStr(mode));
    Serial.println(F("\" }"));
}

void OperatingState::setEmergency(const EmergencyType &e)
{
    Serial.print(F("#E "));
    Serial.print(emergencyTypeToStr(emergency));
    Serial.print(F(" -> "));
    emergency = e;
    Serial.println(emergencyTypeToStr(emergency));

    Serial.print(F(R"({ "emergency" : ")"));
    Serial.print(emergencyTypeToStr(emergency));
    Serial.println(F("\" }"));
}
