#pragma once

#include "core/renderer/renderer.h"

#include "math/type.h"

namespace cadise {

class SurfaceIntersection;
class Ray;
class RGBColor;

// WhittedRenderer is Whitted-style ray tracing renderer
class WhittedRenderer : public Renderer {
public:
    WhittedRenderer(const int32 maxDepth, const int32 sampleNumber);

    void render(const Scene& scene) const override;

private:
    RGBColor _luminance(const Scene& scene, Ray& ray) const;
    RGBColor _reflect(const Scene& scene, Ray& ray, SurfaceIntersection& surfaceIntersection) const;

    int32 _maxDepth;
    int32 _sampleNumber;
};

} // namespace cadise