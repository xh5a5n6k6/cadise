#include "core/integral-tool/directLightEvaluator.h"

#include "core/bsdf/bsdf.h"
#include "core/light/light.h"
#include "core/integral-tool/mis.h"
#include "core/intersector/primitive/primitive.h"
#include "core/ray.h"
#include "core/scene.h"
#include "core/surfaceIntersection.h"

#include "math/constant.h"

namespace cadise {

Spectrum DirectLightEvaluator::evaluate(const Scene& scene, const SurfaceIntersection& surfaceIntersection,
                                        const Bsdf* bsdf, const Light* light) {

    SurfaceIntersection intersection(surfaceIntersection);
    Spectrum directLightRadiance(0.0_r);

    const Vector3R hitPoint  = intersection.surfaceInfo().point();
    const Vector3R hitNormal = intersection.surfaceInfo().shadingNormal();

    // mis using light sampling
    {
        Vector3R lightDir;
        real t;
        real lightPdf;
        Spectrum radiance = light->evaluateSampleRadiance(lightDir, intersection.surfaceInfo(), t, lightPdf);
        intersection.setWo(lightDir);

        // generate shadow ray to do occluded test
        Ray shadowRay = Ray(hitPoint + constant::RAY_EPSILON * hitNormal,
                            lightDir,
                            constant::RAY_EPSILON,
                            t - constant::RAY_EPSILON);

        if (!radiance.isZero() && !scene.isOccluded(shadowRay)) {
            Spectrum reflectance = bsdf->evaluate(intersection);
            real LdotN = lightDir.absDot(hitNormal);
            Spectrum directLightFactor = reflectance * LdotN;

            // if light is delta light, not using mis technique
            if (light->isDeltaLight()) {
                directLightRadiance += directLightFactor * radiance / lightPdf;
            }
            else {
                // calculate bsdf's pdf
                real bsdfPdf = bsdf->evaluatePdfW(intersection);

                directLightRadiance += mis::powerHeuristic(lightPdf, bsdfPdf)
                                       * directLightFactor * radiance / lightPdf;
            }
        }
    } // mis using light sampling

    // mis using bsdf sampling
    {
        if (!light->isDeltaLight()) {
            Spectrum reflectance = bsdf->evaluateSample(intersection);
            real sign = (intersection.wo().dot(hitNormal) < 0.0_r) ? -1.0_r : 1.0_r;

            real LdotN = intersection.wo().absDot(hitNormal);
            Spectrum directLightFactor = reflectance * LdotN;

            Ray sampleRay = Ray(hitPoint + constant::RAY_EPSILON * hitNormal * sign,
                                intersection.wo(),
                                constant::RAY_EPSILON,
                                std::numeric_limits<real>::max());

            if (scene.isIntersecting(sampleRay, intersection)) {
                const Primitive* hitPrimitive = intersection.primitiveInfo().primitive();
                if (hitPrimitive->isEmissive()) {
                    real bsdfPdf = intersection.pdf();
                    Spectrum radiance = hitPrimitive->emittance(sampleRay.direction().composite(), intersection.surfaceInfo());

                    // calcualte emitter's pdf
                    real lightPdf = light->evaluatePdfW(intersection, sampleRay.maxT());

                    directLightRadiance += mis::powerHeuristic(bsdfPdf, lightPdf)
                                           * directLightFactor * radiance / bsdfPdf;
                }
            }
        }
    } // mis using bsdf sampling

    return directLightRadiance;
}

} // namespace cadise