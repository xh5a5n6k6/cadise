#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise { class PathVertex; }
namespace cadise { class Scene; }
namespace cadise { class SubPath; }

namespace cadise
{

class SubPathConnector 
{
public:
    SubPathConnector();

    void connect(
        const Scene&      scene,
        const SubPath&    lightPath,
        const SubPath&    cameraPath,
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