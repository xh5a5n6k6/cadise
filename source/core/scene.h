#pragma once

#include <memory>
#include <vector>

namespace cadise {

class Accelerator;
class Intersector;
class Light;
class Primitive;
class Ray;
class SurfaceIntersection;

class Scene {
public:
    Scene(const std::shared_ptr<Accelerator>& topAccelerator,
          const std::vector<std::shared_ptr<Light>>& lights);

    bool isIntersecting(Ray& ray, SurfaceIntersection& surfaceIntersection) const;
    bool isOccluded(const Ray& ray) const;
    
    const std::vector<std::shared_ptr<Light>>& lights() const;

    void setEnvironmentSphere(const Primitive* const environmentSphere);

private:
    std::shared_ptr<Accelerator> _topAccelerator;
    std::vector<std::shared_ptr<Light>> _lights;

    const Primitive* _environmentSphere;
};

} // namespace cadise