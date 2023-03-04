#include "StorageTransactionResult.h"

StorageLoadResult::StorageLoadResult() :
loaded(0), stored(0), stored_defaults(0), loaded_defaults(0), loaded_crc_mismatch(0),
loaded_version_mismatch(0), loaded_crc_mismatch_after_storing_defaults(0),
loaded_version_mismatch_after_storing_defaults(0), fatal_error(0)
{
}

StorageStoreResult::StorageStoreResult() :
loaded(0), stored(0), loaded_crc_mismatch_after_storing(0),
loaded_version_mismatch_after_storing(0), fatal_error(0)
{
}
