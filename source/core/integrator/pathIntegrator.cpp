#include "core/integrator/pathIntegrator.h"

#include "core/bsdf/bsdf.h"
#include "core/integral-tool/directLightEvaluator.h"
#include "core/integral-tool/russianRoulette.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/category/areaLight.h"
#include "core/ray.h"
#include "core/scene.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"
#include "math/constant.h"

namespace cadise {

PathIntegrator::PathIntegrator(const int32 maxDepth) :
    _maxDepth(maxDepth) {
}

void PathIntegrator::traceRadiance(
    const Scene&    scene, 
    const Ray&      ray,
    Spectrum* const out_radiance) const {

    CADISE_ASSERT(out_radiance);

    Spectrum totalRadiance(0.0_r);
    Spectrum pathWeight(1.0_r);

    // set this flag true at 0 bounce,
    // and update it at each intersection
    // (specular surface for true, non-specular surface for false)
    bool isCountForEmittance = true;

    Ray traceRay = Ray(ray);
    for (int32 bounceTimes = 0; bounceTimes < _maxDepth; ++bounceTimes) {
        SurfaceIntersection intersection;
        if (!scene.isIntersecting(traceRay, intersection)) {
            break;
        }

        const Primitive* primitive = intersection.primitiveInfo().primitive();
        const Bsdf*      bsdf      = primitive->bsdf();

        const Vector3R P  = intersection.surfaceInfo().point();
        const Vector3R Ns = intersection.surfaceInfo().shadingNormal();

        // add emitter's emittance only at first hit-point (0 bounce)
        // or previous hit surface is specular
        const AreaLight* areaLight = primitive->areaLight();
        if (areaLight && isCountForEmittance) {
            const Spectrum emittance = areaLight->emittance(traceRay.direction().reverse(), intersection);
            totalRadiance += pathWeight * emittance;
        }


        // estimate direct light using MIS technique 
        // (only at non-specular surface)
        if (!bsdf->type().isAtLeastOne(BxdfType::SPECULAR_REFLECTION,
                                       BxdfType::SPECULAR_TRANSMISSION)) {

            isCountForEmittance = false;

            real lightPdf;
            const Light* sampleLight = scene.sampleOneLight(&lightPdf);

            const Spectrum directLightRadiance = DirectLightEvaluator::evaluate(scene, intersection,
                                                                          bsdf, sampleLight) / lightPdf;
            totalRadiance += pathWeight * directLightRadiance;
        }
        else {
            isCountForEmittance = true;
        }

        // estimate indirect light with bsdf sampling
        const Spectrum reflectance = bsdf->evaluateSample(intersection);
        const Vector3R L = intersection.wo();

        const real LdotN = L.absDot(Ns);
        pathWeight *= reflectance * LdotN / intersection.pdf();

        if (reflectance.isZero() || intersection.pdf() == 0.0_r) {
            pathWeight = Spectrum(0.0_r);
        }

        // use russian roulette to decide if the ray needs to be kept tracking
        if (bounceTimes > 2) {
            Spectrum newPathWeight;
            if (RussianRoulette::isSurvivedOnNextRound(pathWeight, &newPathWeight)) {
                pathWeight = newPathWeight;
            }
            else {
                break;
            }
        }

        if (pathWeight.isZero()) {
            break;
        }

        traceRay = Ray(P, L);
    }

    *out_radiance = totalRadiance;
}

} // namespace cadise