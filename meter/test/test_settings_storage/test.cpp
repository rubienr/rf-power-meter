#if !defined(ENV_NATIVE)
#include <Arduino.h>
#else
#include <cstring>
#endif
#include "../../lib/settings/Settings.h"
#include "../../lib/storage/DummyStorageDevice.hpp"
#include "../../lib/storage/SettingsStorage.hpp"
#include <unity.h>


struct DummyConstLoadStorageDeviceCrcOk
{
    Settings &get(int, Settings &t) const
    {
        t.sample.separation_ms = 100;
        t.updateCrc();
        return t;
    }
    const Settings &put(int, const Settings &t) { return t; }
};

struct DummyConstLoadStorageDeviceCrcFail
{
    Settings &get(int, Settings &t) const
    {
        t.updateCrc();
        t.crc++;
        return t;
    }
    const Settings &put(int, const Settings &t) { return t; }
};

struct DummyConstLoadStorageDeviceVersionMismatch
{
    Settings &get(int, Settings &t) const
    {
        t.device.version.patch++;
        t.updateCrc();
        return t;
    }
    const Settings &put(int, const Settings &t) { return t; }
};

struct DummyStoreStorageDevice
{
    Settings s;
    Settings &get(int, Settings &t) const
    {
        t = s;
        return t;
    }
    const Settings &put(int, const Settings &t)
    {
        s = t;
        return t;
    }
};

struct DummyStoreStorageDeviceCrcFail
{
    Settings s;
    Settings &get(int, Settings &t) const
    {
        t = s;
        return t;
    }
    const Settings &put(int, const Settings &t)
    {
        s = t;
        s.crc++;
        return t;
    }
};

struct DummyStoreStorageDevicePayloadMismatch
{
    Settings s;
    Settings &get(int, Settings &t) const
    {
        t = s;
        return t;
    }
    const Settings &put(int, const Settings &t)
    {
        s = t;
        s.device.version.patch++;
        s.updateCrc();
        return t;
    }
};

using DummySettingsLoadStorage = SettingsStorage<DummyStorageDevice<Settings>>;
using DummySettingsLoadConstStorageCrcOk = SettingsStorage<DummyConstLoadStorageDeviceCrcOk>;
using DummySettingsLoadConstStorageCrcFail = SettingsStorage<DummyConstLoadStorageDeviceCrcFail>;
using DummySettingsLoadConstStorageVersionMismatch = SettingsStorage<DummyConstLoadStorageDeviceVersionMismatch>;

using DummySettingsStoreStorage = SettingsStorage<DummyStoreStorageDevice>;
using DummySettingsStoreStorageCrcFail = SettingsStorage<DummyStoreStorageDeviceCrcFail>;
using DummySettingsStoreStoragePayloadMismatch = SettingsStorage<DummyStoreStorageDevicePayloadMismatch>;

void test_compute_and_check_crc_01()
{
    Settings s;
    s.sample.separation_ms = 100;
    s.updateCrc();
    TEST_ASSERT(s.checkCrc());
}

void test_compute_and_check_crc_02()
{
    Settings s;
    s.sample.separation_ms = 0xffff;
    TEST_ASSERT_FALSE(s.checkCrc());
}

void test_load()
{
    DummySettingsLoadStorage storage;
    Settings a, b;
    a.sample.separation_ms = 100;
    b.sample.separation_ms = 100;
    storage.load(b);
    TEST_ASSERT(a == b);
}

void test_store()
{
    DummySettingsLoadStorage storage;
    Settings a, b;
    a.sample.separation_ms = 100;
    b = a;

    storage.store(a);

    b.device.configWrites++;
    b.updateCrc();

    TEST_ASSERT(a == b);
}

void test_load_or_init_crc_ok()
{
    Settings a;
    a.sample.separation_ms = 123;

    DummySettingsLoadConstStorageCrcOk storage;
    StorageLoadResult result = storage.loadOrInit(a);

    TEST_ASSERT_EQUAL(100, a.sample.separation_ms.get());
    TEST_ASSERT_EQUAL(1, result.loaded);
    TEST_ASSERT_EQUAL(0, result.stored_defaults);
    TEST_ASSERT_EQUAL(0, result.loaded_defaults);
    TEST_ASSERT_EQUAL(0, result.loaded_crc_mismatch);
    TEST_ASSERT_EQUAL(0, result.loaded_version_mismatch);
    TEST_ASSERT_EQUAL(0, result.loaded_crc_mismatch_after_storing_defaults);
    TEST_ASSERT_EQUAL(0, result.loaded_version_mismatch_after_storing_defaults);
    TEST_ASSERT_EQUAL(0, result.fatal_error);
}

void test_load_or_init_crc_fail()
{
    Settings a;
    a.sample.separation_ms = 123;

    DummySettingsLoadConstStorageCrcFail storage;
    auto result = storage.loadOrInit(a);

    TEST_ASSERT_EQUAL(123, a.sample.separation_ms.get());
    TEST_ASSERT_EQUAL(0, result.loaded);
    TEST_ASSERT_EQUAL(1, result.stored_defaults);
    TEST_ASSERT_EQUAL(1, result.loaded_defaults);
    TEST_ASSERT_EQUAL(1, result.loaded_crc_mismatch);
    TEST_ASSERT_EQUAL(0, result.loaded_version_mismatch);
    TEST_ASSERT_EQUAL(1, result.loaded_crc_mismatch_after_storing_defaults);
    TEST_ASSERT_EQUAL(0, result.loaded_version_mismatch_after_storing_defaults);
    TEST_ASSERT_EQUAL(1, result.fatal_error);
}

void test_load_or_init_version_mismatch()
{
    Settings a;
    a.sample.separation_ms = 123;
    uint16_t patchVersion{ a.device.version.patch };

    DummySettingsLoadConstStorageVersionMismatch storage;
    auto result = storage.loadOrInit(a);

    TEST_ASSERT_EQUAL(123, a.sample.separation_ms.get());
    TEST_ASSERT_EQUAL(patchVersion, a.device.version.patch);
    TEST_ASSERT_EQUAL(0, result.loaded);
    TEST_ASSERT_EQUAL(1, result.stored_defaults);
    TEST_ASSERT_EQUAL(1, result.loaded_defaults);
    TEST_ASSERT_EQUAL(0, result.loaded_crc_mismatch);
    TEST_ASSERT_EQUAL(1, result.loaded_version_mismatch);
    TEST_ASSERT_EQUAL(0, result.loaded_crc_mismatch_after_storing_defaults);
    TEST_ASSERT_EQUAL(1, result.loaded_version_mismatch_after_storing_defaults);
    TEST_ASSERT_EQUAL(1, result.fatal_error);
}

void test_store_or_init_ok()
{
    Settings a;
    a.sample.separation_ms = 123;
    a.device.configWrites = 42;

    DummySettingsStoreStorage storage;
    auto result = storage.storeAndCheck(a);

    TEST_ASSERT_EQUAL(43, a.device.configWrites);
    TEST_ASSERT_EQUAL(1, result.stored);
    TEST_ASSERT_EQUAL(0, result.loaded_crc_mismatch_after_storing);
    TEST_ASSERT_EQUAL(0, result.fatal_error);
}

void test_store_or_init_crc_fail()
{
    Settings a;
    a.sample.separation_ms = 123;
    a.device.configWrites = 42;

    DummySettingsStoreStorageCrcFail storage;
    auto result = storage.storeAndCheck(a);

    TEST_ASSERT_EQUAL(43, a.device.configWrites);
    TEST_ASSERT_EQUAL(0, result.stored);
    TEST_ASSERT_EQUAL(1, result.loaded_crc_mismatch_after_storing);
    TEST_ASSERT_EQUAL(1, result.fatal_error);
}

void test_store_or_init_payload_mismatch()
{
    Settings a;
    a.sample.separation_ms = 123;
    a.device.configWrites = 42;

    DummySettingsStoreStoragePayloadMismatch storage;
    auto result = storage.storeAndCheck(a);

    TEST_ASSERT_EQUAL(43, a.device.configWrites);
    TEST_ASSERT_EQUAL(0, result.stored);
    TEST_ASSERT_EQUAL(0, result.loaded_crc_mismatch_after_storing);
    TEST_ASSERT_EQUAL(1, result.fatal_error);
}

int tests()
{
    UNITY_BEGIN();
    RUN_TEST(test_compute_and_check_crc_01);
    RUN_TEST(test_compute_and_check_crc_02);
    RUN_TEST(test_load);
    RUN_TEST(test_store);
    RUN_TEST(test_load_or_init_crc_ok);
    RUN_TEST(test_load_or_init_crc_fail);
    RUN_TEST(test_load_or_init_version_mismatch);
    RUN_TEST(test_store_or_init_ok);
    RUN_TEST(test_store_or_init_crc_fail);
    RUN_TEST(test_store_or_init_payload_mismatch);
    return UNITY_END();
}

void setUp() {}

void tearDown() {}

#include "../helpers/run-tests.h"
