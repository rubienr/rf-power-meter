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


template <const char *indent> struct _LogSettings
{
    explicit _LogSettings(Stream &outStream) : out(outStream) {}

    void printIdent(uint8_t indentSteps)
    {
        for(; indentSteps > 0; indentSteps--)
            out.print(indent);
    }

    void log(const Version &v, uint8_t indentSteps = 0)
    {
        printIdent(indentSteps);
        out.print(F(R"("major" : ")"));
        out.print(v.major);
        out.println(F("\","));

        printIdent(indentSteps);
        out.print(F(R"("minor" : ")"));
        out.print(v.minor);
        out.println(F("\","));

        printIdent(indentSteps);
        out.print(F(R"("patch" : ")"));
        out.print(v.patch);
        out.println(F("\""));
    }

    void log(const DeviceInfo &d, uint8_t indentSteps = 0)
    {
        printIdent(indentSteps);
        out.println(F("\"version\" : {"));
        log(d.version, indentSteps + 1);
        printIdent(indentSteps);
        out.println(F("},"));

        printIdent(indentSteps);
        out.print(F(R"("configWrites" : ")"));
        out.print(d.configWrites);
        out.println(F("\""));
    }

    void log(const Sample &s, uint8_t indentSteps = 0)
    {
        printIdent(indentSteps);
        out.print(F(R"("separation_ms" : ")"));
        out.print(s.separation_ms.get());
        out.println(F("\""));
    }

    void log(const Render &r, uint8_t indentSteps = 0)
    {
        printIdent(indentSteps);
        out.print(F(R"("separation_ms" : ")"));
        out.print(r.separation_ms.get());
        out.println(F("\""));
    }

    void log(const Settings &s, uint8_t indentSteps = 0)
    {
        printIdent(indentSteps);
        out.println(F("\"device\" : {"));
        log(s.device, indentSteps + 1);
        printIdent(indentSteps);
        out.println(F("},"));

        printIdent(indentSteps);
        out.println(F("\"render\" : {"));
        log(s.render, indentSteps + 1);
        printIdent(indentSteps);
        out.println(F("},"));

        printIdent(indentSteps);
        out.println(F("\"sample\" : {"));
        log(s.sample, indentSteps + 1);
        printIdent(indentSteps);
        out.println(F("},"));

        printIdent(indentSteps);
        out.print(F(R"("crc" : ")"));
        out.print(s.crc);
        out.println(F("\""));
    }
    Stream &out;
};

const char _indent2[]{ "  " };
using LogSettings = _LogSettings<_indent2>;
