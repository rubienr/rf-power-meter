#pragma once
#include "../lib/settings/Settings.h"
#include "../lib/storage/EepromStorageDevice.hpp"
#include "../lib/storage/SettingsStorage.hpp"

struct UiData;
// class U8G2;
class U8X8;
class Stream;
class OperatingState;

struct UiRenderer
{
public:
    UiRenderer(UiData &uiData,
               OperatingState &operatingState,
               SettingsStorage<EepromStorageDevice<Settings>> &settings,
               U8X8 &outDisplay,
               Stream &outSerial);
    void render();

private:
    UiData &data;
    OperatingState &operatingState;
    SettingsStorage<EepromStorageDevice<Settings>> &settings;
    U8X8 &display;
    Stream &serial;
};
