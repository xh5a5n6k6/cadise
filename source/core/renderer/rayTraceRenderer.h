#pragma once

#include "core/renderer.h"

#include "core/color.h"
#include "core/intersection.h"

namespace cadise {

class RayTraceRenderer : public Renderer {
public:
	RayTraceRenderer();
	RayTraceRenderer(int maxDepth, int sampleNumber);

	void render(Scene &scene) override;



private:
	RGBColor _luminance(Scene &scene, Ray &ray, Intersection &intersection);
	RGBColor _reflect(Scene &scene, Ray &ray, Intersection &intersection);

	int _maxDepth;
	int _sampleNumber;
};

} // namespace cadise