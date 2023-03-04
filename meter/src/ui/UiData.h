#pragma once
#include <inttypes.h>
#include <string.h>

enum class CurrentUiContext : uint8_t
{
    Main,
    Setup,
    Default = Main
};

struct UiData
{
    CurrentUiContext uiContext{ CurrentUiContext::Default };
};
