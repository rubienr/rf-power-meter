#if !defined(ENV_NATIVE)
    #include <Arduino.h>
#else
#endif
#include "../../lib/ad7887/AD7887.h"
#include "../../lib/ad7887/registers.h"

#include <inttypes.h>
#include <unity.h>

uint16_t controlRegisterOnAd7887{ 0 };
uint16_t sampleRegisterOnAd7887{ 0 };

void delayMicroSeconds(unsigned int){};
void chipSelectDigitalWrite(uint8_t) {}
void clkDigitalWrite(uint8_t) {}
void controlDigitalWrite(uint8_t digitalValue)
{
    // expect that data is provided from MSB to LSB in same order as AD7887 expects input data
    static uint16_t currentDataBit{ 1 };
    controlRegisterOnAd7887 <<= 1;
    controlRegisterOnAd7887 |= digitalValue ? 1 : 0;
    currentDataBit <<= 1;
    if(currentDataBit == 0) currentDataBit = 1;
}

int sampleDataDigitalRead()
{
    // read from MSB to LSB in the same order as AD7887 would send data
    static uint16_t currentDataBit{ 0x8000 };
    int data{ sampleRegisterOnAd7887 & currentDataBit ? 1 : 0 };
    currentDataBit >>= 1;
    if(currentDataBit == 0) currentDataBit = 0x8000;
    return data;
}

void test_write_control_register_01()
{
    controlRegisterOnAd7887 = 0x0;

    TransmissionData txCtlRegister{};
    txCtlRegister.asUint16 = 0b0000000011111101;

    AD7887 probe(txCtlRegister.asUint16ControlRegister.controlRegister, chipSelectDigitalWrite, clkDigitalWrite,
                 controlDigitalWrite, sampleDataDigitalRead, delayMicroSeconds);
    SampleRegister sample;

    TEST_ASSERT(probe.readSample(sample));
    TEST_ASSERT_EQUAL(0b0000000011111101, controlRegisterOnAd7887);
}

void test_write_control_register_02()
{
    controlRegisterOnAd7887 = 0x0;

    TransmissionData txCtlRegister{};
    txCtlRegister.asUint16 = 0b1010101011111101; // 1st eight bits are ignored

    AD7887 probe(txCtlRegister.asUint16ControlRegister.controlRegister, chipSelectDigitalWrite, clkDigitalWrite,
                 controlDigitalWrite, sampleDataDigitalRead, delayMicroSeconds);
    SampleRegister sample;

    TEST_ASSERT(probe.readSample(sample));
    TEST_ASSERT_EQUAL(0b0000000011111101, controlRegisterOnAd7887);
}

void test_write_control_register_03()
{
    controlRegisterOnAd7887 = 0x0;

    TransmissionData txCtlRegister{};
    txCtlRegister.asUint16 = 0b1111111100000000; // 1st eight bits are ignored

    AD7887 probe(txCtlRegister.asUint16ControlRegister.controlRegister, chipSelectDigitalWrite, clkDigitalWrite,
                 controlDigitalWrite, sampleDataDigitalRead, delayMicroSeconds);
    SampleRegister sample;

    TEST_ASSERT(probe.readSample(sample));
    TEST_ASSERT_EQUAL(0b0000000000000000, controlRegisterOnAd7887);
}

void test_read_sample_register_01()
{
    sampleRegisterOnAd7887 = 0x0;
    TransmissionData txCtlRegister{};

    AD7887 probe(txCtlRegister.asUint16ControlRegister.controlRegister, chipSelectDigitalWrite, clkDigitalWrite,
                 controlDigitalWrite, sampleDataDigitalRead, delayMicroSeconds);
    SampleRegister sample;

    TEST_ASSERT(probe.readSample(sample));
    TEST_ASSERT_EQUAL(0x0, sample.data);
}

void test_read_sample_register_02()
{
    sampleRegisterOnAd7887 = 0b0000000000000001;
    TransmissionData txCtlRegister{};

    AD7887 probe(txCtlRegister.asUint16ControlRegister.controlRegister, chipSelectDigitalWrite, clkDigitalWrite,
                 controlDigitalWrite, sampleDataDigitalRead, delayMicroSeconds);
    SampleRegister sample;

    TEST_ASSERT(probe.readSample(sample));
    TEST_ASSERT_EQUAL(0b0000000000000001, sample.data);
}

void test_read_sample_register_03()
{
    sampleRegisterOnAd7887 = 0b0000111111111101;
    TransmissionData txCtlRegister{};

    AD7887 probe(txCtlRegister.asUint16ControlRegister.controlRegister, chipSelectDigitalWrite, clkDigitalWrite,
                 controlDigitalWrite, sampleDataDigitalRead, delayMicroSeconds);
    SampleRegister sample;

    TEST_ASSERT(probe.readSample(sample));
    TEST_ASSERT_EQUAL(0b0000111111111101, sample.data);
}

void test_read_sample_register_04()
{
    sampleRegisterOnAd7887 = 0b1000000000000000; // 1st four bits must be zero
    TransmissionData txCtlRegister{};

    AD7887 probe(txCtlRegister.asUint16ControlRegister.controlRegister, chipSelectDigitalWrite, clkDigitalWrite,
                 controlDigitalWrite, sampleDataDigitalRead, delayMicroSeconds);
    SampleRegister sample;

    TEST_ASSERT_FALSE(probe.readSample(sample));
}

void test_read_sample_register_05()
{
    sampleRegisterOnAd7887 = 0b0001000000000000; // 1st four bits must be zero
    TransmissionData txCtlRegister{};

    AD7887 probe(txCtlRegister.asUint16ControlRegister.controlRegister, chipSelectDigitalWrite, clkDigitalWrite,
                 controlDigitalWrite, sampleDataDigitalRead, delayMicroSeconds);
    SampleRegister sample;

    TEST_ASSERT_FALSE(probe.readSample(sample));
}

void test_read_sample_register_06()
{
    sampleRegisterOnAd7887 = 0b0000100000000000; // 1st four bits must be zero
    TransmissionData txCtlRegister{};

    AD7887 probe(txCtlRegister.asUint16ControlRegister.controlRegister, chipSelectDigitalWrite, clkDigitalWrite,
                 controlDigitalWrite, sampleDataDigitalRead, delayMicroSeconds);
    SampleRegister sample;

    TEST_ASSERT(probe.readSample(sample));
}

void test_read_sample_write_control_register()
{
    controlRegisterOnAd7887 = 0x0;
    sampleRegisterOnAd7887 = 0b0000101111111111;
    TransmissionData txCtlRegister{};
    txCtlRegister.asUint16 = 0b0000000011111101;

    AD7887 probe(txCtlRegister.asUint16ControlRegister.controlRegister, chipSelectDigitalWrite, clkDigitalWrite,
                 controlDigitalWrite, sampleDataDigitalRead, delayMicroSeconds);
    SampleRegister sample;
    bool success = probe.readSample(sample);

    TEST_ASSERT_EQUAL(true, success);
    TEST_ASSERT_EQUAL(0b0000101111111111, sample.data);
    TEST_ASSERT_EQUAL(0b0000000011111101, controlRegisterOnAd7887);
}
int tests()
{
    UNITY_BEGIN();
    RUN_TEST(test_write_control_register_01);
    RUN_TEST(test_write_control_register_02);
    RUN_TEST(test_write_control_register_03);
    RUN_TEST(test_read_sample_register_01);
    RUN_TEST(test_read_sample_register_02);
    RUN_TEST(test_read_sample_register_03);
    RUN_TEST(test_read_sample_register_04);
    RUN_TEST(test_read_sample_register_05);
    RUN_TEST(test_read_sample_register_06);
    RUN_TEST(test_read_sample_write_control_register);
    return UNITY_END();
}

void setUp() {}

void tearDown() {}

#include "../helpers/run-tests.h"
