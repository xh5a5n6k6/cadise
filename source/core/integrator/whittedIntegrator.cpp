#include "core/integrator/whittedIntegrator.h"

#include "core/bsdf/bsdf.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/light.h"
#include "core/ray.h"
#include "core/scene.h"
#include "core/surfaceIntersection.h"

#include "math/constant.h"

namespace cadise {

WhittedIntegrator::WhittedIntegrator(const int32 maxDepth) :
    _maxDepth(maxDepth) {
}

Spectrum WhittedIntegrator::traceRadiance(const Scene& scene, Ray& ray) const {
    Spectrum totalRadiance(0.0_r);

    SurfaceIntersection intersection;
    if (scene.isIntersecting(ray, intersection)) {
        const Primitive*            hitPrimitive = intersection.primitiveInfo().primitive();
        const std::shared_ptr<Bsdf> hitBsdf      = hitPrimitive->bsdf();
        
        const Vector3R hitPoint  = intersection.surfaceInfo().point();
        const Vector3R hitNormal = intersection.surfaceInfo().geometryNormal();
        
        // add radiance if hitting area light
        totalRadiance += hitPrimitive->emittance(-ray.direction());

        for (uint64 index = 0; index < scene.lights().size(); index++) {
            Vector3R lightDir;
            real t;
            real pdf;
            Spectrum radiance = scene.lights()[index]->evaluateSampleRadiance(lightDir, intersection.surfaceInfo(), t, pdf);
            
            // generate shadow ray to do occluded test
            Ray r = Ray(hitPoint + constant::RAY_EPSILON * hitNormal,
                        lightDir,
                        constant::RAY_EPSILON, 
                        t - constant::RAY_EPSILON);

            if (scene.isOccluded(r)) {
                continue;
            }

            Spectrum reflectance = hitBsdf->evaluate(intersection);
            if (!reflectance.isZero()) {
                real LdotN = lightDir.absDot(hitNormal);
                totalRadiance += reflectance * radiance * LdotN / pdf;
            }
        }

        if (ray.depth() + 1 < _maxDepth) {
            totalRadiance += _radianceOnScattering(scene, ray, intersection);
        }
    }
    else {
        // TODO : add environment map influence
    }

    return totalRadiance;
}

Spectrum WhittedIntegrator::_radianceOnScattering(const Scene& scene, Ray& ray, SurfaceIntersection& intersection) const {
    Spectrum sampleRadiance(0.0_r);

    const Primitive*            primitive = intersection.primitiveInfo().primitive();
    const std::shared_ptr<Bsdf> bsdf      = primitive->bsdf();

    intersection.setWi(-ray.direction());

    Spectrum reflectance = bsdf->evaluateSample(intersection);
    if (!reflectance.isZero()) {
        Vector3R hitPoint = intersection.surfaceInfo().point();
        Vector3R hitNormal = intersection.surfaceInfo().geometryNormal();
        real sign = (intersection.wo().dot(hitNormal) < 0.0_r) ? -1.0_r : 1.0_r;
        Ray r = Ray(hitPoint + constant::RAY_EPSILON * hitNormal * sign,
                    intersection.wo(),
                    constant::RAY_EPSILON,
                    std::numeric_limits<real>::max(),
                    ray.depth() + 1);
        real LdotN = intersection.wo().absDot(hitNormal);
        sampleRadiance = reflectance * traceRadiance(scene, r) * LdotN / intersection.pdf();
    }

    return sampleRadiance;
}

} // namespace cadise