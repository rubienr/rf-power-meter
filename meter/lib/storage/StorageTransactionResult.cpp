#include "StorageTransactionResult.h"

namespace storage
{

LoadResult::LoadResult() :
loaded(0), stored_defaults(0), loaded_defaults(0), loaded_crc_mismatch(0), loaded_version_mismatch(0),
loaded_crc_mismatch_after_storing_defaults(0), loaded_version_mismatch_after_storing_defaults(0), fatal_error(0)
{
}

StoreResult::StoreResult() : stored(0), loaded_crc_mismatch_after_storing(0), fatal_error(0) {}

} // namespace storage
