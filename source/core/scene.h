#pragma once

#include <memory>
#include <vector>

namespace cadise {

class Accelerator;
class SurfaceIntersection;
class Light;
class Ray;

class Scene {
public:
    Scene(const std::shared_ptr<Accelerator>& accelerator,
          const std::vector<std::shared_ptr<Light>>& lights);

    bool isIntersecting(Ray& ray, SurfaceIntersection& surfaceIntersection) const;
    bool isOccluded(const Ray& ray) const;
    
    const std::vector<std::shared_ptr<Light>>& lights() const;

private:
    std::shared_ptr<Accelerator> _topAccelerator;
    std::vector<std::shared_ptr<Light>> _lights;
};

} // namespace cadise