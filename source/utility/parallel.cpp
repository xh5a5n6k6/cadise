#include "utility/parallel.h"

#include "fundamental/assertion.h"
#include "math/math.h"

#include <thread>
#include <vector>

namespace cadise {

void Parallel::execute(
    const std::size_t numWorks,
    const std::size_t numWorkers,
    const Work&       work) {

    const std::size_t averageWorkload = (numWorks + numWorkers - 1) / numWorkers;

    std::vector<std::thread> workers(numWorkers);
    for (std::size_t i = 0; i < numWorkers; ++i) {
        const std::size_t beginIndex = averageWorkload * i;
        const std::size_t endIndex   = math::min(averageWorkload * (i + 1), numWorks);

        workers[i] = std::thread(work, i, beginIndex, endIndex);
    }

    for (std::size_t i = 0; i < numWorkers; ++i) {
        CADISE_ASSERT(workers[i].joinable());

        workers[i].join();
    }
}

} // namespace cadise