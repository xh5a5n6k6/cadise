#include "core/integrator/pathIntegrator.h"

#include "core/bsdf/bsdf.h"
#include "core/integral-tool/directLightEvaluator.h"
#include "core/integral-tool/lightSampler.h"
#include "core/integral-tool/russianRoulette.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/areaLight.h"
#include "core/ray.h"
#include "core/scene.h"
#include "core/surfaceIntersection.h"
#include "math/constant.h"

namespace cadise {

PathIntegrator::PathIntegrator(const int32 maxDepth) :
    _maxDepth(maxDepth) {
}

Spectrum PathIntegrator::traceRadiance(const Scene& scene, const Ray& ray) const {
    Spectrum totalRadiance(0.0_r);
    Spectrum pathWeight(1.0_r);
    int32 bounceTimes = 0;

    // set this flag true at 0 bounce,
    // and update it at each intersection
    // (specular surface for true, non-specular surface for false)
    bool isCountForEmittance = true;

    Ray traceRay = Ray(ray);
    while (bounceTimes < _maxDepth) {
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

            const std::vector<std::shared_ptr<Light>>& lights = scene.lights();

            real lightPdf;
            std::size_t lightIndex = LightSampler::sampleOneLight<LightSamplePolicy::UNIFORM>(lights, &lightPdf);
            const Light* sampleLight = lights[lightIndex].get();

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
        const real sign = (L.dot(Ns) < 0.0_r) ? -1.0_r : 1.0_r;

        const real LdotN = L.absDot(Ns);
        pathWeight *= reflectance * LdotN / intersection.pdf();

        if (reflectance.isZero() || intersection.pdf() == 0.0_r) {
            pathWeight = Spectrum(0.0_r);
        }
        if (pathWeight.isZero()) {
            break;
        }

        bounceTimes += 1;
        Ray sampleRay(P, L);

        // use russian roulette to decide if the ray needs to be kept tracking
        if (bounceTimes > 2) {
            pathWeight = russianRoulette::weightOnNextPath(pathWeight);

            if (!pathWeight.isZero()) {
                traceRay = sampleRay;
            }
            else {
                break;
            }
        }
        else {
            traceRay = sampleRay;
        }
    }

    return totalRadiance;
}

} // namespace cadise