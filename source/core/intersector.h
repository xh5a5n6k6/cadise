#pragma once

namespace cadise {

class Intersection;
class Ray;
class RGBColor;

// Intersector represents an object that is intersectable (includs primitives and lights)
class Intersector {
public:
	virtual bool isIntersecting(Ray &ray, Intersection &intersection) = 0;
	virtual bool isOccluded(Ray &ray) = 0;
	virtual RGBColor emittance() = 0;

	//virtual void getBSDF() = 0;
};

} // namespace cadise