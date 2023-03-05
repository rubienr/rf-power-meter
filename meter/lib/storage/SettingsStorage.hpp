#pragma once

#include "../configuration.h"
#include "../settings/Settings.h"
#include "StorageTransactionResult.h"
#include <inttypes.h>

template <typename StorageDevice_t> class SettingsStorage
{
public:
    /**
     * Loads Settings from EEPROM (without CRC check).
     * @param settings data out
     * @param storage
     */
    void load(Settings &s) const { storage.get(EEPROM_START_ADDRESS, s); }

    /**
     * Stores the given Settings to EEPROM (without CRC check).
     * This method updates the
     * - CRC and the
     * - total writes fields.
     * @param settings
     * @param storage
     */
    void store(Settings &settings)
    {
        settings.device.configWrites++;
        settings.crc = settings.computeCrc();
        storage.put(EEPROM_START_ADDRESS, settings);
    }

    /**
     * Store, re-load and check the CRC from EEPROM vs. the given settings argument.
     * This function applies store() and thus may update the given settings argument.
     * @param defaultSettings
     * @param storage
     * @return false if the re-loaded value's CRC does not match
     */
    StorageLoadResult loadOrInit(Settings &defaultSettings)
    {
        StorageLoadResult result;
        Settings loadedSettings;
        loadedSettings.sample.separation_ms = 99;
        loadedSettings.render.separation_ms = 91;

        load(loadedSettings);
        if(loadedSettings.checkCrc())
        {
            // OK: CRC okay and version as expected
            if(loadedSettings.device.version == defaultSettings.device.version)
            {
                defaultSettings = loadedSettings;
                result.loaded = true;
                return result;
            }
            // OK: CRC okay but version not as expected -> store default values
            else
            {
                result.loaded_version_mismatch = true;
                store(defaultSettings);
                result.stored_defaults = true;
            }
        }
        // ERROR: CRC failed -> store default values
        else
        {
            result.loaded_crc_mismatch = true;
            store(defaultSettings);
            result.stored_defaults = true;
        }

        load(loadedSettings);
        result.loaded_defaults = true;

        if(loadedSettings.checkCrc())
        { // OK: CRC check passed after default settings have been re-stored with default valued
            if(loadedSettings.device.version == defaultSettings.device.version)
            { // OK: version check passed
                defaultSettings = loadedSettings;
                result.loaded = true;
                return result;
            }
            else
            {
                // FATAL ERROR, version check failed: most likely EEPROM segments broken
                // Note: try to manually update EEPROM_START_ADDRESS to mitigate this issue.
                result.loaded_version_mismatch_after_storing_defaults = true;
                result.fatal_error = true;
                return result;
            }
        }
        else
        {
            // FATAL ERROR, CRC failed: most likely EEPROM segments broken
            // Note: try to manually update EEPROM_START_ADDRESS to mitigate this issue.
            result.loaded_crc_mismatch_after_storing_defaults = true;
            result.fatal_error = true;
            return result;
        }
    }

    /**
     * Tries to load from EEPROM and does error handling on CRC mismatch or version mismatch.
     * This function applies store() and thus may update the given settings argument.
     * @param defaultSettings
     * @param storage
     * @return false if error handling is not possible, true (regardless of error handling attempts)
     * if loaded CRC is valid and the loaded version matches the expected version
     */
    StorageStoreResult storeAndCheck(Settings &defaultSettings)
    {
        StorageStoreResult result;
        Settings loadedSettings;

        store(defaultSettings);
        load(loadedSettings);

        if(loadedSettings.checkCrc())
        { // OK: CRC check passed

            if(defaultSettings == loadedSettings)
            {
                result.stored = true;
                return result;
            }
            else
            {
                // WTF (what a terrible failure)!
                result.fatal_error = true;
                return result;
            }
        }
        else
        {
            // FATAL ERROR, CRC failed: most likely EEPROM segments broken
            // Note: try to manually update EEPROM_START_ADDRESS to mitigate this issue.
            result.loaded_crc_mismatch_after_storing = true;
            result.fatal_error = true;
            return result;
        }
    }

protected:
    StorageDevice_t storage{};
};
