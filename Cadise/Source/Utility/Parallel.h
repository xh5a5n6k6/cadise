#pragma once

#include <functional>

namespace cadise
{

class Parallel
{
private:
    using Work = std::function
        <
            void(
                const std::size_t workerId,
                const std::size_t workBeginIndex,
                const std::size_t workEndIndex)
        >;

public:
    static void execute(
        const std::size_t numWorks,
        const std::size_t numWorkers,
        const Work&       work);
};

} // namespace cadise