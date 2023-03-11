#pragma once

#include "Core/RenderDatabase.h"

#include <memory>

// forward declaration
namespace cadise 
{
    class CSDResource;
    class Film;
    class Renderer;
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

    void consumeResource(const std::shared_ptr<CSDResource>& resource);
    void preRender();
    void render() const;

    void setThreadCount(const std::size_t threadCount);

private:
    EngineConfig   _config;
    RenderDatabase _renderDatabase;

    std::shared_ptr<Renderer> _renderer;
    std::shared_ptr<Film>     _film;
};

} // namespace cadise