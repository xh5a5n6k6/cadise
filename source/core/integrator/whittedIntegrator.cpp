#include "core/integrator/whittedIntegrator.h"

#include "core/bsdf/bsdf.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/areaLight.h"
#include "core/ray.h"
#include "core/scene.h"
#include "core/surfaceIntersection.h"

#include "math/constant.h"

namespace cadise {

WhittedIntegrator::WhittedIntegrator(const int32 maxDepth) :
    _maxDepth(maxDepth) {
}

Spectrum WhittedIntegrator::traceRadiance(const Scene& scene, const Ray& ray) const {
    Spectrum totalRadiance(0.0_r);
    Spectrum pathWeight(1.0_r);
    int32 bounceTimes = 0;

    Ray traceRay(ray);
    while (bounceTimes < _maxDepth) {
        SurfaceIntersection intersection;
        if (!scene.isIntersecting(traceRay, intersection)) {
            // TODO : add environment light influence

            break;
        }

        const Primitive* hitPrimitive = intersection.primitiveInfo().primitive();
        const Bsdf*      hitBsdf      = hitPrimitive->bsdf();

        const Vector3R hitPoint  = intersection.surfaceInfo().point();
        const Vector3R hitNormal = intersection.surfaceInfo().shadingNormal();

        const bool isSpecular = hitBsdf->type().isAtLeastOne(BsdfType(BxdfType::SPECULAR_REFLECTION),
                                                             BsdfType(BxdfType::SPECULAR_TRANSMISSION));

        // add radiance if hitting area light
        const AreaLight* areaLight = hitPrimitive->areaLight();
        if (areaLight) {
            totalRadiance += areaLight->emittance(traceRay.direction().reverse(), intersection.surfaceInfo());
        }

        // add direct light only at non-specular surface
        if (!isSpecular) {
            const std::vector<std::shared_ptr<Light>>& lights = scene.lights();
            for (std::size_t i = 0; i < lights.size(); ++i) {
                Vector3R lightDir;
                real t;
                real pdf;
                const Spectrum emitRadiance = lights[i]->evaluateSampleRadiance(lightDir, intersection.surfaceInfo(), t, pdf);

                // generate shadow ray to do occluded test
                const Ray shadowRay(hitPoint + constant::RAY_EPSILON * hitNormal,
                                    lightDir,
                                    constant::RAY_EPSILON,
                                    t - constant::RAY_EPSILON);

                if (scene.isOccluded(shadowRay)) {
                    continue;
                }

                const Spectrum reflectance = hitBsdf->evaluate(intersection);
                if (!reflectance.isZero() && pdf > 0.0_r) {
                    const real LdotN = lightDir.absDot(hitNormal);
                    const Spectrum directLightRadiance = reflectance * emitRadiance * LdotN / pdf;

                    totalRadiance += pathWeight * directLightRadiance;
                }
            }

            // once hitting non-specular surface, we
            // calculate direct lighting and then exit
            break;
        }

        // only trace next ray at specular surface
        // according to bsdf sampling
        else {
            const Spectrum reflectance = hitBsdf->evaluateSample(intersection);
            if (!reflectance.isZero()) {
                const real sign  = (intersection.wo().dot(hitNormal) < 0.0_r) ? -1.0_r : 1.0_r;
                const real LdotN = intersection.wo().absDot(hitNormal);
                const Ray sampleRay(hitPoint + constant::RAY_EPSILON * hitNormal * sign,
                                    intersection.wo(),
                                    constant::RAY_EPSILON,
                                    std::numeric_limits<real>::max());
                
                pathWeight *= reflectance * LdotN / intersection.pdf();
                if (!pathWeight.isZero()) {
                    traceRay = sampleRay;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }

        bounceTimes += 1;
    }

    return totalRadiance;
}

} // namespace cadise