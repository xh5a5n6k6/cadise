#pragma once

#include "math/type/fundamentalType.h"

#include <memory>
#include <vector>

namespace cadise {

class Accelerator;
class Intersector;
class Light;
class LightCluster;
class Primitive;
class Ray;
class SurfaceIntersection;

class Scene {
public:
    Scene(const std::shared_ptr<Accelerator>&  topAccelerator,
          const std::shared_ptr<LightCluster>& lightCluster);

    bool isIntersecting(Ray& ray, SurfaceIntersection& surfaceIntersection) const;
    bool isOccluded(const Ray& ray) const;
    
    const Light* sampleOneLight(real* const out_pdf) const;

    void setBackgroundSphere(const Primitive* const backgroundSphere);

private:
    std::shared_ptr<Accelerator>  _topAccelerator;
    std::shared_ptr<LightCluster> _lightCluster;

    const Primitive* _backgroundSphere;
};

} // namespace cadise