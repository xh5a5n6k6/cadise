#pragma once

#include "core/camera.h"
#include "core/shape.h"

#include "core/intersector.h"
#include "core/light.h"

namespace cadise {

class Scene {
public:
	Scene();
	Scene(std::vector<std::shared_ptr<Intersector> > intersectors, 
		  std::vector<std::shared_ptr<Light> > lights, 
		  std::shared_ptr<Camera> camera);

	bool isIntersecting(Ray &ray, Intersection &intersection);
	bool isOccluded(Ray &ray);

	std::vector<std::shared_ptr<Intersector> > _intersectors;
	std::vector<std::shared_ptr<Light> > _lights;
	std::shared_ptr<Camera> _camera;
};

} // namespace cadise