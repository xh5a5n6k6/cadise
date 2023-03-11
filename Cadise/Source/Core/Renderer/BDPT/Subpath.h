#pragma once

#include "Core/Renderer/BDPT/PathVertex.h"

#include <vector>

// forward declaration
namespace cadise
{
    class Camera;
    class ConnectEvent;
    class Scene;
}

namespace cadise 
{

class Subpath 
{
public:
    // used for s=0 situation
    // (it doesn't support t=0 situation currently)
    static Subpath emptyPath();

    // used for s=1 or t=1 situation
    static Subpath oneVertexPath(const PathVertex& vertex);

public:
    explicit Subpath(const std::size_t maxPathLength);

    PathVertex& operator[] (const std::size_t index);
    const PathVertex& operator[] (const std::size_t index) const;

    void addVertex(const PathVertex& vertex);

    void connectCamera(
        const Scene&                     scene, 
        const Camera* const              camera,
        std::vector<ConnectEvent>* const out_events) const;

    void connectLight(
        const Scene&    scene,
        Spectrum* const out_radiance) const;

    std::size_t length() const;

private:
    std::vector<PathVertex> _vertices;
};

} // namespace cadise