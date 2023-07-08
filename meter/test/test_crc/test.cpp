#if !defined(ENV_NATIVE)
    #include <Arduino.h>
#else
    #include <cstring>
#endif
#include "../../lib/settings/Settings.h"
#include "../lib/crc/crc.h"
#include <unity.h>

using namespace settings;

void test_crc_from_string()
{
    const uint8_t d0[]{ "asdf<crc123>" };
    const uint8_t d1[]{ "asdf<crc100>" };
    TEST_ASSERT_EQUAL(computeCrc(d0, sizeof(d0) - 1 - strlen("<crc123>")), computeCrc(d1, sizeof(d1) - 1 - strlen("<crc100>")));
}

void test_crc_settings_equal()
{
    Settings s0;
    s0.device.configWrites = 100;
    Settings s1;
    s1.device.configWrites = 100;
    TEST_ASSERT_EQUAL(computeCrc(s0.asPtr(), sizeof(s0) - sizeof(s0.crc)), computeCrc(s1.asPtr(), sizeof(s1) - sizeof(s1.crc)));
}

void test_crc_settings_not_equal()
{
    Settings s0;
    s0.device.configWrites = 100;
    Settings s1;
    s1.device.configWrites = 101;
    TEST_ASSERT_NOT_EQUAL(computeCrc(s0.asPtr(), sizeof(s0) - sizeof(s0.crc)), computeCrc(s1.asPtr(), sizeof(s1) - sizeof(s1.crc)));
}

int tests()
{
    UNITY_BEGIN();
    RUN_TEST(test_crc_from_string);
    RUN_TEST(test_crc_settings_equal);
    RUN_TEST(test_crc_settings_not_equal);
    return UNITY_END();
}

void setUp() {}

void tearDown() {}

#include "../helpers/run-tests.h"
