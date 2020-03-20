#pragma once

#include <functional>

namespace cadise {

class Parallel {
public:
    static void parallelWork(
        const std::size_t totalWorks,
        const std::size_t workerNumber,
        const std::function<void(const std::size_t workBeginIndex,
                                 const std::size_t workEndIndex)>& work);
};

} // namespace cadise