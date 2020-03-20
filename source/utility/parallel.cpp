#include "utility/parallel.h"

#include "fundamental/assertion.h"
#include "math/math.h"

#include <thread>
#include <vector>

namespace cadise {

void Parallel::parallelWork(
    const std::size_t totalWorks,
    const std::size_t workerNumber,
    const std::function<void(const std::size_t workBeginIndex,
                             const std::size_t workEndIndex)>& work) {

    const std::size_t averageWorkload = (totalWorks + workerNumber - 1) / workerNumber;

    std::vector<std::thread> workers(workerNumber);
    for (std::size_t i = 0; i < workerNumber; ++i) {
        const std::size_t beginIndex = averageWorkload * i;
        const std::size_t endIndex   = math::min(averageWorkload * (i + 1), totalWorks);

        workers[i] = std::thread(work, beginIndex, endIndex);
    }

    for (std::size_t i = 0; i < workerNumber; ++i) {
        CADISE_ASSERT(workers[i].joinable());

        workers[i].join();
    }
}

} // namespace cadise