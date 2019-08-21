#include "core/integrator/category/purePathIntegrator.h"

#include "core/bsdf/bsdf.h"
#include "core/intersector/primitive/primitive.h"
#include "core/ray.h"
#include "core/integral-tool/russianRoulette.h"
#include "core/scene.h"
#include "core/surfaceIntersection.h"

#include "math/constant.h"

namespace cadise {

PurePathIntegrator::PurePathIntegrator(const int32 maxDepth) :
    _maxDepth(maxDepth) {
}

Spectrum PurePathIntegrator::traceRadiance(const Scene& scene, Ray& ray) const {
    Spectrum totalRadiance(0.0_r);
    Spectrum pathWeight(1.0_r);

    Ray traceRay(ray);
    while (traceRay.depth() <= _maxDepth) {
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

        Ray sampleRay = Ray(hitPoint + constant::RAY_EPSILON * hitNormal * sign,
                            intersection.wo(),
                            constant::RAY_EPSILON,
                            std::numeric_limits<real>::max(),
                            traceRay.depth() + 1);

        if(sampleRay.depth() >= 3) {
            pathWeight = russianRoulette::weightOnNextPath(pathWeight);

            if (pathWeight.isZero()) {
                break;
            }
            else {
                traceRay = sampleRay;
            }
        }
        else {
            traceRay = sampleRay;
        }
    }
    
    return totalRadiance;
}

} // namespace cadise