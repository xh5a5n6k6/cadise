#pragma once

#include "Core/Spectrum/Spectrum.h"

// forward declaration
namespace cadise
{
    class Camera;
    class Ray;
    class Scene;
    class Subpath;

    enum class ETransportMode;
}

namespace cadise
{

class SubpathBuilder
{
public:
    explicit SubpathBuilder(const std::size_t maxPathLength);

    void setCamera(const Camera* const camera);

    void buildLightPath(
        const Scene&   scene,
        Subpath* const out_lightPath) const;

    void buildCameraPath(
        const Scene&    scene,
        const Vector2D& filmPosition,
        Subpath* const  out_cameraPath,
        Spectrum* const out_zeroBounceRadiance) const;

private:
    void _buildSubpathCompletely(
        const ETransportMode mode,
        const Scene&         scene,
        const Ray&           firstRay,
        const Spectrum&      secondVertexThroughput,
        const real           secondVertexForwardPdfW,
        Subpath* const       out_subpath,
        Spectrum* const      out_zeroBoundRadiance) const;

    std::size_t   _maxPathLength;
    const Camera* _camera;
};

} // namespace cadise