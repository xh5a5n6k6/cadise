#pragma once

#include "core/renderer/renderer.h"

namespace cadise {

class Intersection;
class Ray;
class RGBColor;

// WhittedRenderer is for Whitted-style ray tracing renderer
class WhittedRenderer : public Renderer {
public:
    WhittedRenderer(int maxDepth, int sampleNumber);

    void render(Scene &scene) override;

private:
    RGBColor _luminance(Scene &scene, Ray &ray, Intersection &intersection);
    RGBColor _reflect(Scene &scene, Ray &ray, Intersection &intersection);

    int _maxDepth;
    int _sampleNumber;
};

} // namespace cadise