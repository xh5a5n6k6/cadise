#include "core/integrator/naivePathIntegrator.h"

#include "core/bsdf/bsdf.h"
#include "core/intersector/primitive/primitive.h"
#include "core/ray.h"
#include "core/integral-tool/russianRoulette.h"
#include "core/scene.h"
#include "core/surfaceIntersection.h"

#include "math/constant.h"

namespace cadise {

NaivePathIntegrator::NaivePathIntegrator(const int32 maxDepth) :
    _maxDepth(maxDepth) {
}

Spectrum NaivePathIntegrator::traceRadiance(const Scene& scene, const Ray& ray) const {
    Spectrum totalRadiance(0.0_r);
    Spectrum pathWeight(1.0_r);
    int32 bounceTimes = 0;

    Ray traceRay = Ray(ray);
    while (bounceTimes < _maxDepth) {
        SurfaceIntersection intersection;
        if (!scene.isIntersecting(traceRay, intersection)) {
            // TODO : add environment light

            break;
        }

        const Primitive* hitPrimitive = intersection.primitiveInfo().primitive();
        const Bsdf*      hitBsdf      = hitPrimitive->bsdf().get();

        const Vector3R hitPoint  = intersection.surfaceInfo().point();
        const Vector3R hitNormal = intersection.surfaceInfo().shadingNormal();

        if (hitPrimitive->isEmissive()) {
            Spectrum emittance = hitPrimitive->emittance(traceRay.direction().composite(), intersection.surfaceInfo());
            totalRadiance += pathWeight * emittance;
        }

        Spectrum reflectance = hitBsdf->evaluateSample(intersection);
        real sign = (intersection.wo().dot(hitNormal) < 0.0_r) ? -1.0_r : 1.0_r;

        real LdotN = intersection.wo().absDot(hitNormal);
        pathWeight *= reflectance * LdotN / intersection.pdf();

        if (pathWeight.isZero()) {
            break;
        }

        bounceTimes += 1;
        Ray sampleRay = Ray(hitPoint + constant::RAY_EPSILON * hitNormal * sign,
                            intersection.wo(),
                            constant::RAY_EPSILON,
                            std::numeric_limits<real>::max());

        // use russian roulette to decide if the ray needs to be kept tracking
        if(bounceTimes > 2) {
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