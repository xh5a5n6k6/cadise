#pragma once

#include "core/renderer/renderer.h"

#include "core/spectrum/spectrum.h"

namespace cadise {

class Ray;
class Sampler;
class SubPath;
enum class TransportMode;

class BdptRenderer : public Renderer {
public:
    BdptRenderer(const std::shared_ptr<Sampler>& sampler);

    void render(const Scene& scene) const override;

private:
    void _buildLightPath(
        const Scene&   scene,
        SubPath* const out_lightPath) const;

    void _buildCameraPath(
        const Scene&    scene,
        const Vector2R& filmNdcPosition,
        SubPath* const  out_cameraPath,
        Spectrum* const out_zeroBounceRadiance) const;

    void _buildSubPathCompletely(
        const TransportMode& mode,
        const Scene&         scene,
        const Ray&           firstRay,
        const Spectrum&      secondVertexThroughput,
        const real           secondVertexForwardPdfW,
        SubPath* const       out_subPath,
        Spectrum* const      out_zeroBoundRadiance) const;

    void _connectSubPath(
        const Scene&      scene,
        const SubPath&    lightPath, 
        const SubPath&    cameraPath,
        const std::size_t s,
        const std::size_t t,
        Spectrum* const   out_radiance) const;

    std::shared_ptr<Sampler> _sampler;

    const static std::size_t MAX_PATH_LENGTH = 32;
};

} // namespace cadise