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
    WhittedRenderer(int32 maxDepth, int32 sampleNumber);

    void render(Scene &scene) override;

private:
    RGBColor _luminance(Scene &scene, Ray &ray, Intersection &intersection);
    RGBColor _reflect(Scene &scene, Ray &ray, Intersection &intersection);

    int32 _maxDepth;
    int32 _sampleNumber;
};

} // namespace cadise