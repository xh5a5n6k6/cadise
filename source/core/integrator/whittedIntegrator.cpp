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

    Ray traceRay(ray);
    for (int32 bounceTimes = 0; bounceTimes < _maxDepth; ++bounceTimes) {
        SurfaceIntersection intersection;
        if (!scene.isIntersecting(traceRay, intersection)) {
            break;
        }

        const Primitive* primitive = intersection.primitiveInfo().primitive();
        const Bsdf*      bsdf      = primitive->bsdf();

        const Vector3R P  = intersection.surfaceInfo().point();
        const Vector3R Ns = intersection.surfaceInfo().shadingNormal();

        const bool isSpecular = bsdf->type().isAtLeastOne(BxdfType::SPECULAR_REFLECTION,
                                                          BxdfType::SPECULAR_TRANSMISSION);

        // add radiance if hitting area light
        const AreaLight* areaLight = primitive->areaLight();
        if (areaLight) {
            totalRadiance += areaLight->emittance(traceRay.direction().reverse(), intersection);
        }

        // add direct light only at non-specular surface
        if (!isSpecular) {
            const std::vector<std::shared_ptr<Light>>& lights = scene.lights();
            for (std::size_t i = 0; i < lights.size(); ++i) {
                Vector3R L;
                real t;
                real pdf;
                const Spectrum emitRadiance = lights[i]->evaluateSampleRadiance(L, intersection.surfaceInfo(), t, pdf);

                // generate shadow ray to do occluded test
                const Ray shadowRay(P,
                                    L,
                                    constant::RAY_EPSILON,
                                    t - constant::RAY_EPSILON);

                if (scene.isOccluded(shadowRay)) {
                    continue;
                }

                const Spectrum reflectance = bsdf->evaluate(intersection);
                if (!reflectance.isZero() && pdf > 0.0_r) {
                    const real LdotN = L.absDot(Ns);
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
            const Spectrum reflectance = bsdf->evaluateSample(intersection);
            if (!reflectance.isZero()) {
                const Vector3R L = intersection.wo();
                const real LdotN = L.absDot(Ns);
                
                pathWeight *= reflectance * LdotN / intersection.pdf();
                if (pathWeight.isZero()) {
                    break;
                }
                
                traceRay = Ray(P, L);
            }
            else {
                break;
            }
        }
    }

    return totalRadiance;
}

} // namespace cadise