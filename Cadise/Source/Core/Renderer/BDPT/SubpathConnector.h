#pragma once

#include "Core/Spectrum/Spectrum.h"

// forward declaration
namespace cadise
{
    class PathVertex;
    class Scene;
    class Subpath;
}

namespace cadise
{

class SubpathConnector
{
public:
    SubpathConnector();

    void connect(
        const Scene&      scene,
        const Subpath&    lightPath,
        const Subpath&    cameraPath,
        const std::size_t s,
        const std::size_t t,
        Spectrum* const   out_radiance) const;

private:
    bool _canConnect(
        const Scene&      scene,
        const PathVertex& lightEndpoint,
        const PathVertex& cameraEndpoint,
        real* const       out_connectG) const;
};

} // namespace cadise