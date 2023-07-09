#include "pb_encoder.h"
#if defined(HAS_ENCODER)


static void onRisingSignalA()
{
    const bool signalA{ 0 != digitalRead(ENCODER_SIGNAL_A_PIN) };
    const bool signalB{ 0 != digitalRead(ENCODER_SIGNAL_B_PIN) };

    if(signalA)
    {
        if(signalB) pbEncoderHandle->counter--;
        else pbEncoderHandle->counter++;
    }
    else
    {
        if(signalB) pbEncoderHandle->counter++;
        else pbEncoderHandle->counter--;
    }
}

static void onButtonPressed() { pbEncoderHandle->isPbPressed = !digitalRead(ENCODER_PUSHBUTTON_PIN); }

void PbEncoder_setup()
{
    pinMode(ENCODER_SIGNAL_B_PIN, INPUT_PULLUP);
    pinMode(ENCODER_SIGNAL_A_PIN, INPUT_PULLUP);
    pinMode(ENCODER_PUSHBUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ENCODER_SIGNAL_A_PIN), onRisingSignalA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_PUSHBUTTON_PIN), onButtonPressed, CHANGE);
}
void PbEncoder_teardown()
{
    detachInterrupt(digitalPinToInterrupt(ENCODER_PUSHBUTTON_PIN));
    detachInterrupt(digitalPinToInterrupt(ENCODER_SIGNAL_A_PIN));
    pinMode(ENCODER_PUSHBUTTON_PIN, INPUT);
    pinMode(ENCODER_SIGNAL_A_PIN, INPUT);
    pinMode(ENCODER_SIGNAL_B_PIN, INPUT);
}

#endif // HAS_ENCODER
