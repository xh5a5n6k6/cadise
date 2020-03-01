#include "core/integral-tool/directLightEvaluator.h"

#include "core/bsdf/bsdf.h"
#include "core/integral-tool/mis.h"
#include "core/integral-tool/sample/directLightSample.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/category/areaLight.h"
#include "core/ray.h"
#include "core/scene.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"
#include "math/constant.h"

namespace cadise {

Spectrum DirectLightEvaluator::evaluate(
    const Scene&               scene, 
    const SurfaceIntersection& surfaceIntersection,
    const Bsdf*                bsdf, 
    const Light*               light) {

    CADISE_ASSERT(bsdf);
    CADISE_ASSERT(light);

    SurfaceIntersection intersection(surfaceIntersection);
    Spectrum directLightRadiance(0.0_r);

    const Vector3R P  = intersection.surfaceInfo().point();
    const Vector3R Ns = intersection.surfaceInfo().shadingNormal();
    const Vector3R Ng = intersection.surfaceInfo().geometryNormal();

    if (bsdf->type().isExactOne(BxdfType::ABSORB)) {
        return directLightRadiance;
    }

    // mis using light sampling
    {
        DirectLightSample directLightSample;
        directLightSample.setTargetPosition(P);

        light->evaluateDirectSample(&directLightSample);

        if (directLightSample.isValid()) {
            const Vector3R LVector  = directLightSample.emitPosition() - P;
            const real     distance = LVector.length();

            CADISE_ASSERT_GT(distance, 0.0_r);

            const Vector3R L = LVector / distance;
            intersection.setWo(L);

            // generate shadow ray to do occluded test
            Ray shadowRay(P, L, constant::RAY_EPSILON, distance - constant::RAY_EPSILON);

            if (!scene.isOccluded(shadowRay)) {
                const Spectrum& radiance  = directLightSample.radiance();
                const real      lightPdfW = directLightSample.pdfW();

                const Spectrum reflectance       = bsdf->evaluate(intersection);
                const Spectrum directLightFactor = reflectance * L.absDot(Ns);

                // if light is delta light, not using mis technique
                if (light->isDeltaLight()) {
                    directLightRadiance += radiance * directLightFactor / lightPdfW;
                }
                else {
                    const real bsdfPdfW  = bsdf->evaluatePdfW(intersection);
                    const real misWeight = Mis<MisMode::POWER>::weight(lightPdfW, bsdfPdfW);

                    directLightRadiance += misWeight * radiance * directLightFactor / lightPdfW;
                }
            }
        }
    } // mis using light sampling

    // mis using bsdf sampling
    {
        if (!light->isDeltaLight()) {
            const Spectrum  reflectance = bsdf->evaluateSample(intersection);
            const Vector3R& L = intersection.wo();

            if (!reflectance.isZero()) {

                CADISE_ASSERT(!L.isZero());

                Ray sampleRay(P, L);

                if (scene.isIntersecting(sampleRay, intersection)) {

                    const AreaLight* areaLight = intersection.primitiveInfo().primitive()->areaLight();
                    if (areaLight == light) {
                        const Spectrum directLightFactor = reflectance * L.absDot(Ns);
                        const Spectrum radiance = areaLight->emittance(intersection);

                        const real bsdfPdfW = intersection.pdf();
                        const real lightPdfW = areaLight->evaluateDirectPdfW(intersection, P);
                        const real misWeight = Mis<MisMode::POWER>::weight(bsdfPdfW, lightPdfW);

                        directLightRadiance += misWeight * radiance * directLightFactor / bsdfPdfW;
                    }
                }
            }
        }
    } // mis using bsdf sampling

    return directLightRadiance;
}

} // namespace cadise