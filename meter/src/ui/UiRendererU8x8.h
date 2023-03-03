#pragma once

struct UiData;
class U8X8;
class Stream;

struct UiRendererU8X8
{
public:
    UiRendererU8X8(UiData &uiData, U8X8 &outDisplay, Stream &outSerial);
    void render() const {}

private:
    UiData &data;
    U8X8 &display;
    Stream &serial;
};
