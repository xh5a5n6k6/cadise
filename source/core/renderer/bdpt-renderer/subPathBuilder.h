#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class Camera;
class Ray;
class Scene;
class SubPath;
enum class ETransportMode;

class SubPathBuilder {
public:
    explicit SubPathBuilder(const std::size_t maxPathLength);

    void setCamera(const Camera* const camera);

    void buildLightPath(
        const Scene&   scene,
        SubPath* const out_lightPath) const;

    void buildCameraPath(
        const Scene&    scene,
        const Vector2R& filmNdcPosition,
        SubPath* const  out_cameraPath,
        Spectrum* const out_zeroBounceRadiance) const;

private:
    void _buildSubPathCompletely(
        const ETransportMode& mode,
        const Scene&          scene,
        const Ray&            firstRay,
        const Spectrum&       secondVertexThroughput,
        const real            secondVertexForwardPdfW,
        SubPath* const        out_subPath,
        Spectrum* const       out_zeroBoundRadiance) const;

    std::size_t   _maxPathLength;
    const Camera* _camera;
};

} // namespace cadise