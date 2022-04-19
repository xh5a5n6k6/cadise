#include "api/globalStorage.h"

namespace cadise
{

std::mutex& GlobalStorage::_getMutex()
{
    static std::mutex mutex;

    return mutex;
}

} // namespace cadise