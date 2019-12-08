#include "core/integral-tool/directLightEvaluator.h"

#include "core/bsdf/bsdf.h"
#include "core/light/areaLight.h"
#include "core/integral-tool/mis.h"
#include "core/intersector/primitive/primitive.h"
#include "core/ray.h"
#include "core/scene.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"
#include "math/constant.h"

namespace cadise {

Spectrum DirectLightEvaluator::evaluate(const Scene& scene, const SurfaceIntersection& surfaceIntersection,
                                        const Bsdf* bsdf, const Light* light) {

    CADISE_ASSERT(bsdf);
    CADISE_ASSERT(light);

    SurfaceIntersection intersection(surfaceIntersection);
    Spectrum directLightRadiance(0.0_r);

    const Vector3R P  = intersection.surfaceInfo().point();
    const Vector3R Ns = intersection.surfaceInfo().shadingNormal();

    if (bsdf->type().isExactOne(BxdfType::ABSORB)) {
        return directLightRadiance;
    }

    // mis using light sampling
    {
        Vector3R L;
        real t;
        real lightPdf;
        const Spectrum radiance = light->evaluateSampleRadiance(L, intersection.surfaceInfo(), t, lightPdf);
        intersection.setWo(L);

        // generate shadow ray to do occluded test
        Ray shadowRay(P + constant::RAY_EPSILON * Ns,
                      L,
                      constant::RAY_EPSILON,
                      t - constant::RAY_EPSILON);

        if (!radiance.isZero() && !scene.isOccluded(shadowRay)) {
            const Spectrum reflectance = bsdf->evaluate(intersection);
            const real LdotN = L.absDot(Ns);
            const Spectrum directLightFactor = reflectance * LdotN;

            // if light is delta light, not using mis technique
            if (light->isDeltaLight()) {
                directLightRadiance += directLightFactor * radiance / lightPdf;
            }
            else {
                // calculate bsdf's pdf
                const real bsdfPdf = bsdf->evaluatePdfW(intersection);

                directLightRadiance += mis::powerHeuristic(lightPdf, bsdfPdf)
                                       * directLightFactor * radiance / lightPdf;
            }
        }
    } // mis using light sampling

    // mis using bsdf sampling
    {
        if (!light->isDeltaLight()) {
            const Spectrum reflectance = bsdf->evaluateSample(intersection);
            const Vector3R L = intersection.wo();
            const real sign = (L.dot(Ns) < 0.0_r) ? -1.0_r : 1.0_r;

            const real LdotN = L.absDot(Ns);
            const Spectrum directLightFactor = reflectance * LdotN;

            Ray sampleRay(P + constant::RAY_EPSILON * Ns * sign,
                          L);

            if (!reflectance.isZero() && 
                scene.isIntersecting(sampleRay, intersection)) {
                
                const AreaLight* areaLight = intersection.primitiveInfo().primitive()->areaLight();
                if (areaLight == light) {
                    const real bsdfPdf = intersection.pdf();
                    const Spectrum radiance = areaLight->emittance(sampleRay.direction().reverse(), intersection);

                    // calcualte emitter's pdf
                    const real lightPdf = areaLight->evaluatePdfW(intersection, sampleRay.maxT());

                    directLightRadiance += mis::powerHeuristic(bsdfPdf, lightPdf)
                                           * directLightFactor * radiance / bsdfPdf;
                }
            }
        }
    } // mis using bsdf sampling

    return directLightRadiance;
}

} // namespace cadise