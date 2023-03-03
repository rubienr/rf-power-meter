#if defined(ENV_NATIVE)

int main(int argc, char **argv) { return 0; }

#else

#include "settings/Settings.h"
#include "states/OperatingState.h"
#include "ui/UiData.h"
#include "ui/UiRendererU8x8.h"
#include <Arduino.h>
#include <U8x8lib.h>
#include <elapsedMillis.h>

struct Resources
{
    struct EarlyInitializer
    {
        EarlyInitializer()
        {
            Serial.begin(115200, SERIAL_8N1);
            while(!Serial) {}
            Serial.println("early init ...");
        }
    } _;

    UiData uiData{};
    U8X8_SSD1306_128X64_NONAME_HW_I2C display{ U8X8_PIN_NONE, SCL, SDA };
    UiRendererU8X8 renderer{ uiData, display, Serial };

    Settings settings{};
    OperatingState operatingState{};

    elapsedMillis sampleTimer;
    elapsedMillis renderTimer;


    void setup()
    {
        Serial.println("setup ...");

        display.begin();
        display.setPowerSave(0);
        display.setFont(u8x8_font_chroma48medium8_r);
        display.clear();


        Serial.println("setup: done");
    }

    void process()
    {
        switch(operatingState.emergency)
        {
        case EmergencyType::HaltOnUnrecoverableError:
        case EmergencyType::UnknownError:
            doHalt();
            break;
        default:
            break;
        }

        switch(operatingState.mode)
        {
        case OperatingMode::Measure:
            if(sampleTimer > settings.sample.sample_separation_ms.value())
            {
                doSample();
                sampleTimer = 0;
            }
            if(renderTimer > settings.sample.sample_separation_ms.value())
            {
                doRender();
                renderTimer = 0;
            }
            break;

        case OperatingMode::Idle:
            if(renderTimer > settings.sample.sample_separation_ms.value())
            {
                doRender();
                renderTimer = 0;
            }
            break;


        default:
            break;
        }
    }

    void doSample() {}

    void doRender() {}
    void doHalt()
    {
        operatingState.emergency = EmergencyType::HaltOnUnrecoverableError;
        renderer.render();
    }
} r;


void setup(void) { r.setup(); }

void loop(void) { r.process(); }
#endif
