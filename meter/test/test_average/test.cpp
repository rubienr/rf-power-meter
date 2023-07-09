#if !defined(ENV_NATIVE)
    #include <Arduino.h>
#else
#endif
#include "../../lib/average/Average.h"

#include <inttypes.h>
#include <unity.h>

using namespace avg;


void test_avg_default()
{
    Average32x16 avg;
    Average32x16::entry_type value;
    avg.get(value);
    TEST_ASSERT_EQUAL(0, value);
}


void test_avg_clear()
{
    Average32x16 avg;
    Average32x16::entry_type value;
    avg.put(10);
    avg.clear();
    avg.get(value);
    TEST_ASSERT_EQUAL(0, value);
}

void test_avg_one_entry()
{
    Average32x16 avg;
    Average32x16::entry_type value;
    avg.put(10);
    avg.get(value);
    TEST_ASSERT_EQUAL(0, value);
}

void test_avg_two_entries()
{
    Average32x16 avg;
    Average32x16::entry_type value;
    avg.put(8);
    avg.put(8);
    avg.get(value);
    TEST_ASSERT_EQUAL(1, value); // (8 + 8) / 32 = 0.5
}


void test_avg_full_queue()
{
    Average32x16 avg;
    Average32x16::entry_type value;
    for(uint8_t i = 0; i < 32; i++)
    {
        avg.put(65535);
    }
    avg.get(value);
    TEST_ASSERT_EQUAL(65535, value);
}

void test_avg_min_max_two_entries()
{
    Average32x16 avg;
    Average32x16::entry_type value, min, max;
    avg.put(123);
    avg.put(1234);
    avg.get(value, min, max);
    TEST_ASSERT_EQUAL(min, 123);
    TEST_ASSERT_EQUAL(max, 1234);
}

void test_avg_min_max_full_queue()
{
    Average32x16 avg;
    Average32x16::entry_type value, min, max;
    for(uint8_t i = 0; i < 32; i++)
    {
        if(0 == (i % 2)) avg.put(65535);
        else avg.put(1234);
    }
    avg.get(value, min, max);
    TEST_ASSERT_EQUAL(33385, value);
    TEST_ASSERT_EQUAL(1234, min);
    TEST_ASSERT_EQUAL(65535, max);
}


void test_avg_min_max_queue_overflow()
{
    Average32x16 avg;
    Average32x16::entry_type value, min, max;
    for(uint8_t i = 0; i < 32; i++)
    {
        avg.put(65535);
    }
    avg.get(value, min, max);
    TEST_ASSERT_EQUAL(65535, value);
    TEST_ASSERT_EQUAL(65535, min);
    TEST_ASSERT_EQUAL(65535, max);

    avg.put(123);
    avg.put(1234);

    avg.get(value, min, max);
    TEST_ASSERT_EQUAL(61481, value);
    TEST_ASSERT_EQUAL(123, min);
    TEST_ASSERT_EQUAL(1234, max);
}

int tests()
{
    UNITY_BEGIN();
    RUN_TEST(test_avg_default);
    RUN_TEST(test_avg_clear);
    RUN_TEST(test_avg_one_entry);
    RUN_TEST(test_avg_two_entries);
    RUN_TEST(test_avg_full_queue);
    RUN_TEST(test_avg_min_max_two_entries);
    RUN_TEST(test_avg_min_max_full_queue);
    RUN_TEST(test_avg_min_max_queue_overflow);
    return UNITY_END();
}

void setUp() {}

void tearDown() {}

#include "../helpers/run-tests.h"
