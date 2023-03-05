#pragma once
#include "Settings.h"

#if !defined(ENV_NATIVE)
#include <Arduino.h>
#else
struct Stream
{
    void print(const void *) {}
    void println(const void *) {}
    void print(const uint8_t &) {}
    void println(const uint8_t &) {}
    void print(const uint16_t &) {}
    void println(const uint16_t &) {}
};
#endif


template <const char *ident> struct _LogSettings
{
    _LogSettings(Stream &outStream) : out(outStream) {}

    void printIdent(uint8_t identSteps)
    {
        for(; identSteps > 0; identSteps--)
            out.print(ident);
    }

    void log(const Version &v, uint8_t identSteps = 0)
    {
        printIdent(identSteps);
        out.print(F(R"("major" : ")"));
        out.print(v.major);
        out.println(F("\","));

        printIdent(identSteps);
        out.print(F(R"("minor" : ")"));
        out.print(v.minor);
        out.println(F("\","));

        printIdent(identSteps);
        out.print(F(R"("patch" : ")"));
        out.print(v.patch);
        out.println(F("\""));
    }

    void log(const DeviceInfo &d, uint8_t identSteps = 0)
    {
        printIdent(identSteps);
        out.println(F("\"version\" : {"));
        log(d.version, identSteps + 1);
        printIdent(identSteps);
        out.println(F("},"));

        printIdent(identSteps);
        out.print(F(R"("configWrites" : ")"));
        out.print(d.configWrites);
        out.println(F("\""));
    }

    void log(const Sample &s, uint8_t identSteps = 0)
    {
        printIdent(identSteps);
        out.print(F(R"("separation_ms" : ")"));
        out.print(s.separation_ms.get());
        out.println(F("\""));
    }

    void log(const Render &r, uint8_t identSteps = 0)
    {
        printIdent(identSteps);
        out.print(F(R"("separation_ms" : ")"));
        out.print(r.separation_ms.get());
        out.println(F("\""));
    }

    void log(const Settings &s, uint8_t identSteps = 0)
    {
        printIdent(identSteps);
        out.println(F("\"device\" : {"));
        log(s.device, identSteps + 1);
        printIdent(identSteps);
        out.println(F("},"));

        printIdent(identSteps);
        out.println(F("\"render\" : {"));
        log(s.render, identSteps + 1);
        printIdent(identSteps);
        out.println(F("},"));

        printIdent(identSteps);
        out.println(F("\"sample\" : {"));
        log(s.sample, identSteps + 1);
        printIdent(identSteps);
        out.println(F("},"));

        printIdent(identSteps);
        out.print(F(R"("crc" : ")"));
        out.print(s.crc);
        out.println(F("\""));
    }
    Stream &out;
};

const char _ident2[]{ "  " };
using LogSettings = _LogSettings<_ident2>;
