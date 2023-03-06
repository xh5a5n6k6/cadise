#pragma once

#include "core/renderDatabase.h"

#include <memory>

// forward declaration
namespace cadise 
{
    class Film;
    class Renderer;
    class SdData;
}

namespace cadise 
{

class Engine
{
private:
    struct EngineConfig
    {
        std::size_t numThreads = 1;
    };

public:
    Engine();

    void consumeResource(const std::shared_ptr<SdData>& sdData);
    void prepareRender();
    void render() const;

    void setThreadCount(const std::size_t threadCount);

private:
    EngineConfig   _config;
    RenderDatabase _renderDatabase;

    std::shared_ptr<Renderer> _renderer;
    std::shared_ptr<Film>     _film;
};

} // namespace cadise