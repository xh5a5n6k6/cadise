#include "core/integrator/pathIntegrator.h"

#include "core/bsdf/bsdf.h"
#include "core/integral-tool/directLightEvaluator.h"
#include "core/integral-tool/lightSampler.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/areaLight.h"
#include "core/ray.h"
#include "core/integral-tool/russianRoulette.h"
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
            // TODO : add environment light

            break;
        }

        const Primitive* hitPrimitive = intersection.primitiveInfo().primitive();
        const Bsdf*      hitBsdf      = hitPrimitive->bsdf();

        const Vector3R hitPoint  = intersection.surfaceInfo().point();
        const Vector3R hitNormal = intersection.surfaceInfo().shadingNormal();

        // add emitter's emittance only at first hit-point (0 bounce)
        // or previous hit surface is specular
        const AreaLight* areaLight = hitPrimitive->areaLight();
        if (areaLight && isCountForEmittance) {
            const Spectrum emittance = areaLight->emittance(traceRay.direction().reverse(), intersection.surfaceInfo());
            totalRadiance += pathWeight * emittance;
        }

        // estimate direct light using MIS technique 
        // (but only at non-specular surface)
        if (!hitBsdf->type().isAtLeastOne(BsdfType(BxdfType::SPECULAR_REFLECTION),
                                          BsdfType(BxdfType::SPECULAR_TRANSMISSION))) {

            isCountForEmittance = false;

            const std::vector<std::shared_ptr<Light>>& lights = scene.lights();

            real lightPdf;
            std::size_t lightIndex = LightSampler::sampleOneLight<LightSamplePolicy::UNIFORM>(lights, &lightPdf);
            const Light* sampleLight = lights[lightIndex].get();

            const Spectrum directLightRadiance = DirectLightEvaluator::evaluate(scene, intersection,
                                                                          hitBsdf, sampleLight) / lightPdf;
            totalRadiance += pathWeight * directLightRadiance;
        }
        else {
            isCountForEmittance = true;
        }

        // estimate indirect light with bsdf sampling
        const Spectrum reflectance = hitBsdf->evaluateSample(intersection);
        const real sign = (intersection.wo().dot(hitNormal) < 0.0_r) ? -1.0_r : 1.0_r;

        const real LdotN = intersection.wo().absDot(hitNormal);
        pathWeight *= reflectance * LdotN / intersection.pdf();

        if (pathWeight.isZero()) {
            break;
        }

        bounceTimes += 1;
        Ray sampleRay(hitPoint + constant::RAY_EPSILON * hitNormal * sign,
                      intersection.wo(),
                      constant::RAY_EPSILON,
                      std::numeric_limits<real>::max());

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