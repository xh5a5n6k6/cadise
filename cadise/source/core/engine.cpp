#include "core/engine.h"

#include "fundamental/logger/logger.h"

#include <thread>

namespace cadise 
{

namespace
{
    const Logger logger("Engine");
}


Engine::Engine()
{
    _numThreads = 1;
}

void Engine::consumeResource(const std::shared_ptr<SdData>& sdData)
{
    _renderDatabase.setUpData(sdData);
}

void Engine::render()
{
    _renderDatabase.prepareRender();
    _renderDatabase.render();
}

void Engine::setThreadCount(const std::size_t threadCount)
{
    if (threadCount > std::thread::hardware_concurrency())
    {
        logger.log("Specified thread count exceeds the maximum supported number, skip this setting!");
        return;
    }

    _numThreads = threadCount;
}

} // namespace cadise