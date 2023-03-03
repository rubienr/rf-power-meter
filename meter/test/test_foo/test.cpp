#if !defined(ENV_NATIVE)
#include <Arduino.h>
#endif

#include <unity.h>

void test_foo() { TEST_ASSERT_EQUAL(42, 42); }

int tests()
{
    UNITY_BEGIN();
    RUN_TEST(test_foo);
    return UNITY_END();
}

void setUp() {}

void tearDown() {}

#include "../helpers/run-tests.h"
