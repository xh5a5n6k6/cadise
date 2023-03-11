#pragma once

#include "Math/Type/MathType.h"

#include <memory>

// forward declaration
namespace cadise
{
    class Accelerator;
    class Intersector;
    class Light;
    class LightCluster;
    class Primitive;
    class Ray;
    class SurfaceIntersection;
}

namespace cadise
{

class Scene 
{
public:
    Scene(
        const std::shared_ptr<Accelerator>&  topAccelerator,
        const std::shared_ptr<LightCluster>& lightCluster);

    void evaluateBound(AABB3R* const out_bound) const;

    bool isIntersecting(Ray& ray, SurfaceIntersection& surfaceIntersection) const;
    bool isOccluded(const Ray& ray) const;
    
    const Light* sampleOneLight(real* const out_pdf) const;
    real evaluatePickLightPdf(const Light* const light) const;

    void setBackgroundSphere(const Primitive* const backgroundSphere);
    void unsetBackgroundSphere();

private:
    std::shared_ptr<Accelerator>  _topAccelerator;
    std::shared_ptr<LightCluster> _lightCluster;

    const Primitive* _backgroundSphere;
};

} // namespace cadise