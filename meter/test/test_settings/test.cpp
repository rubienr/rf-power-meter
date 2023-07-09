#if !defined(ENV_NATIVE)
    #include <Arduino.h>
#else
#endif
#include "../../lib/settings/ConstrainedNumericValue.hpp"
#include <unity.h>

using namespace settings;

void test_constrained_numeric_value_in_range_01()
{
    ConstrainedNumericValue<uint16_t, 7, 7, 7> constrainedValue;
    TEST_ASSERT_EQUAL(7, constrainedValue.get());
}

void test_constrained_numeric_value_in_range_02()
{
    ConstrainedNumericValue<uint16_t, 7, 3, 11> constrainedValue;
    TEST_ASSERT_EQUAL(7, constrainedValue.get());
}

void test_constrained_numeric_assign_value_below_range_01()
{
    ConstrainedNumericValue<uint16_t, 7, 3, 11> constrainedValue;
    TEST_ASSERT_EQUAL(7, constrainedValue.get());
    constrainedValue = 2;
    TEST_ASSERT_EQUAL(3, constrainedValue.get());
}

void test_constrained_numeric_assign_value_below_range_02()
{
    ConstrainedNumericValue<uint16_t, 7, 7, 7> constrainedValue;
    TEST_ASSERT_EQUAL(7, constrainedValue.get());
    constrainedValue = 6;
    TEST_ASSERT_EQUAL(7, constrainedValue.get());
}

void test_constrained_numeric_assign_value_above_range_01()
{
    ConstrainedNumericValue<uint16_t, 7, 3, 11> constrainedValue;
    TEST_ASSERT_EQUAL(7, constrainedValue.get());
    constrainedValue = 12;
    TEST_ASSERT_EQUAL(11, constrainedValue.get());
}

void test_constrained_numeric_assign_value_above_range_02()
{
    ConstrainedNumericValue<uint16_t, 7, 7, 7> constrainedValue;
    TEST_ASSERT_EQUAL(7, constrainedValue.get());
    constrainedValue = 8;
    TEST_ASSERT_EQUAL(7, constrainedValue.get());
}
int tests()
{
    UNITY_BEGIN();
    RUN_TEST(test_constrained_numeric_value_in_range_01);
    RUN_TEST(test_constrained_numeric_value_in_range_02);
    RUN_TEST(test_constrained_numeric_assign_value_below_range_01);
    RUN_TEST(test_constrained_numeric_assign_value_below_range_02);
    RUN_TEST(test_constrained_numeric_assign_value_above_range_01);
    RUN_TEST(test_constrained_numeric_assign_value_above_range_02);
    return UNITY_END();
}

void setUp() {}

void tearDown() {}

#include "../helpers/run-tests.h"
