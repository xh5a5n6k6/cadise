#include "core/integrator/category/whittedIntegrator.h"

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
    if (!scene.isIntersecting(ray, intersection)) {
        // TODO : add environment light influence

        return totalRadiance;
    }

    const Primitive* hitPrimitive = intersection.primitiveInfo().primitive();
    const Bsdf*      hitBsdf      = hitPrimitive->bsdf().get();
        
    const Vector3R hitPoint  = intersection.surfaceInfo().point();
    const Vector3R hitNormal = intersection.surfaceInfo().shadingNormal();
     
    const bool isSpecular = hitBsdf->type().isAtLeastOne(BsdfType(BxdfType::SPECULAR_REFLECTION),
                                                         BsdfType(BxdfType::SPECULAR_TRANSMISSION));

    // add radiance if hitting area light
    if (hitPrimitive->isEmissive()) {
        totalRadiance += hitPrimitive->emittance(ray.direction().composite(), intersection.surfaceInfo());
    }
    
    // add direct light only at non-specular surface
    if (!isSpecular) {
        for (uint64 index = 0; index < scene.lights().size(); index++) {
            Vector3R lightDir;
            real t;
            real pdf;
            Spectrum radiance = scene.lights()[index]->evaluateSampleRadiance(lightDir, intersection.surfaceInfo(), t, pdf);

            // generate shadow ray to do occluded test
            Ray shadowRay = Ray(hitPoint + constant::RAY_EPSILON * hitNormal,
                                lightDir,
                                constant::RAY_EPSILON,
                                t - constant::RAY_EPSILON);

            if (scene.isOccluded(shadowRay)) {
                continue;
            }

            Spectrum reflectance = hitBsdf->evaluate(intersection);
            if (!reflectance.isZero() && pdf > 0.0_r) {
                real LdotN = lightDir.absDot(hitNormal);
                totalRadiance += reflectance * radiance * LdotN / pdf;
            }
        }
    }

    // only recursive trace at specular surface
    if (isSpecular && ray.depth() + 1 < _maxDepth) {
        totalRadiance += _radianceOnScattering(scene, ray, intersection);
    }

    return totalRadiance;
}

Spectrum WhittedIntegrator::_radianceOnScattering(const Scene& scene, Ray& ray, SurfaceIntersection& intersection) const {
    Spectrum sampleRadiance(0.0_r);

    const Primitive* primitive = intersection.primitiveInfo().primitive();
    const Bsdf*      bsdf      = primitive->bsdf().get();

    Spectrum reflectance = bsdf->evaluateSample(intersection);
    if (!reflectance.isZero()) {
        Vector3R hitPoint  = intersection.surfaceInfo().point();
        Vector3R hitNormal = intersection.surfaceInfo().shadingNormal();
        real sign = (intersection.wo().dot(hitNormal) < 0.0_r) ? -1.0_r : 1.0_r;
        Ray sampleRay = Ray(hitPoint + constant::RAY_EPSILON * hitNormal * sign,
                            intersection.wo(),
                            constant::RAY_EPSILON,
                            std::numeric_limits<real>::max(),
                            ray.depth() + 1);
        real LdotN = intersection.wo().absDot(hitNormal);
        sampleRadiance = reflectance * traceRadiance(scene, sampleRay) * LdotN / intersection.pdf();
    }

    return sampleRadiance;
}

} // namespace cadise