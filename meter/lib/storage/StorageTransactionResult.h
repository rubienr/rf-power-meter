#pragma once

#include <inttypes.h>

namespace storage
{


struct LoadResult
{
    uint8_t loaded : 1;
    uint8_t stored_defaults : 1;
    uint8_t loaded_defaults : 1;
    uint8_t loaded_crc_mismatch : 1;
    uint8_t loaded_version_mismatch : 1;
    uint8_t loaded_crc_mismatch_after_storing_defaults : 1;
    uint8_t loaded_version_mismatch_after_storing_defaults : 1;
    uint8_t fatal_error : 1;

    LoadResult();
};

struct StoreResult
{
    uint8_t stored : 1;
    uint8_t loaded_crc_mismatch_after_storing : 1;
    uint8_t fatal_error : 1;

    StoreResult();
};

} // namespace storage
