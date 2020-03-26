#pragma once

#include <functional>

namespace cadise {

class Parallel {
public:
    static void parallelWork(
        const std::size_t numWorks,
        const std::size_t numWorkers,
        const std::function<void(
            const std::size_t workerId,
            const std::size_t workBeginIndex,
            const std::size_t workEndIndex)>& work);
};

} // namespace cadise