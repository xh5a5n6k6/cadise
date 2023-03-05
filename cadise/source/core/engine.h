#pragma once

#include "core/renderDatabase.h"

#include <memory>

// forward declaration
namespace cadise 
{ 
    class SdData;
}

namespace cadise 
{

class Engine
{
public:
    Engine();

    void consumeResource(const std::shared_ptr<SdData>& sdData);
    void render();

    void setThreadCount(const std::size_t threadCount);

private:
    RenderDatabase _renderDatabase;
    std::size_t    _numThreads;
};

} // namespace cadise