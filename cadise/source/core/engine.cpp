#include "core/engine.h"

#include "fundamental/logger/logger.h"

#include <thread>

#include "renderer/renderer.h"

namespace cadise 
{

namespace
{
    const Logger logger("Engine");
}

Engine::Engine() = default;

void Engine::consumeResource(const std::shared_ptr<SdData>& sdData)
{
    _renderDatabase.setUpData(sdData);
}

void Engine::prepareRender()
{
    _renderer = _renderDatabase.prepareRender();
    _renderer->setWorkerCount(_config.numThreads);
}

void Engine::render() const
{
    _renderer->render();
}

void Engine::setThreadCount(const std::size_t threadCount)
{
    if (threadCount > std::thread::hardware_concurrency())
    {
        logger.log("Specified thread count exceeds the maximum supported number, skip this setting!");
        return;
    }

    _config.numThreads = threadCount;
}

} // namespace cadise