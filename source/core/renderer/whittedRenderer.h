#pragma once

#include "core/renderer/renderer.h"

#include "math/type.h"

namespace cadise {

class Intersection;
class Ray;
class RGBColor;

// WhittedRenderer is for Whitted-style ray tracing renderer
class WhittedRenderer : public Renderer {
public:
    WhittedRenderer(const int32 maxDepth, const int32 sampleNumber);

    void render(const Scene scene) const override;

private:
    RGBColor _luminance(const Scene scene, Ray &ray, Intersection &intersection) const;
    RGBColor _reflect(const Scene scene, Ray &ray, Intersection &intersection) const;

    int32 _maxDepth;
    int32 _sampleNumber;
};

} // namespace cadise