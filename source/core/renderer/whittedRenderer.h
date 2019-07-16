#pragma once

#include "core/renderer/renderer.h"

#include "core/spectrum/spectrum.h"

namespace cadise {

class SurfaceIntersection;
class Ray;

// WhittedRenderer is Whitted-style ray tracing renderer
class WhittedRenderer : public Renderer {
public:
    WhittedRenderer(const int32 maxDepth, const int32 sampleNumber);

    void render(const Scene& scene) const override;

private:
    Spectrum _radiance(const Scene& scene, Ray& ray) const;
    Spectrum _radianceOnScattering(const Scene& scene, Ray& ray, SurfaceIntersection& surfaceIntersection) const;

    int32 _maxDepth;
    int32 _sampleNumber;
};

} // namespace cadise