#include "API/GlobalStorage.h"

namespace cadise
{

std::mutex& GlobalStorage::_getMutex()
{
    static std::mutex mutex;

    return mutex;
}

} // namespace cadise