#include "core/integrator/pathIntegrator.h"

#include "core/bsdf/bsdf.h"
#include "core/integral-tool/directLightEvaluator.h"
#include "core/integral-tool/lightSampler.h"
#include "core/intersector/primitive/primitive.h"
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
    bool isPreviousHitSpecular = false;

    Ray traceRay = Ray(ray);
    while (traceRay.depth() < _maxDepth) {
        bool isCountForEmittance = traceRay.depth() == 0 || isPreviousHitSpecular;
        SurfaceIntersection intersection;
        if (!scene.isIntersecting(traceRay, intersection)) {
            // TODO : add environment light
            if (isCountForEmittance) {
            
            }

            break;
        }

        const Primitive* hitPrimitive = intersection.primitiveInfo().primitive();
        const Bsdf*      hitBsdf      = hitPrimitive->bsdf().get();

        const Vector3R hitPoint  = intersection.surfaceInfo().point();
        const Vector3R hitNormal = intersection.surfaceInfo().shadingNormal();

        // add emitter's emittance only at first hit-point (0 bounce)
        // or previous hit surface is specular
        if (hitPrimitive->isEmissive() && isCountForEmittance) {
            Spectrum emittance = hitPrimitive->emittance(traceRay.direction().composite(), intersection.surfaceInfo());
            totalRadiance += pathWeight * emittance;
        }

        // estimate direct light using MIS technique 
        // (but only at non-specular surface)
        if (!hitBsdf->type().isAtLeastOne(BsdfType(BxdfType::SPECULAR_REFLECTION),
                                          BsdfType(BxdfType::SPECULAR_TRANSMISSION))) {

            isPreviousHitSpecular = false;

            real lightPdf;
            std::size_t lightIndex = LightSampler::sampleOneLight<LightSamplePolicy::UNIFORM>(scene, &lightPdf);
            const Light* sampleLight = scene.lights()[lightIndex].get();

            Spectrum directLightRadiance = DirectLightEvaluator::evaluate(scene, intersection,
                                                                          hitBsdf, sampleLight) / lightPdf;
            totalRadiance += pathWeight * directLightRadiance;
        }
        else {
            isPreviousHitSpecular = true;
        }

        // estimate indirect light with bsdf sampling
        Spectrum reflectance = hitBsdf->evaluateSample(intersection);
        real sign = (intersection.wo().dot(hitNormal) < 0.0_r) ? -1.0_r : 1.0_r;

        real LdotN = intersection.wo().absDot(hitNormal);
        pathWeight *= reflectance * LdotN / intersection.pdf();

        if (pathWeight.isZero()) {
            break;
        }

        Ray sampleRay = Ray(hitPoint + constant::RAY_EPSILON * hitNormal * sign,
                            intersection.wo(),
                            constant::RAY_EPSILON,
                            std::numeric_limits<real>::max(),
                            traceRay.depth() + 1);

        // use russian roulette to decide if the ray needs to be kept tracking
        if (sampleRay.depth() > 2) {
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