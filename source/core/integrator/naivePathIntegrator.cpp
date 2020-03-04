#include "core/integrator/naivePathIntegrator.h"

#include "core/integral-tool/russianRoulette.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/category/areaLight.h"
#include "core/ray.h"
#include "core/scene.h"
#include "core/surface/bsdf/bsdf.h"
#include "core/surface/transportInfo.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"
#include "math/constant.h"

namespace cadise {

NaivePathIntegrator::NaivePathIntegrator(const int32 maxDepth) :
    _maxDepth(maxDepth) {
}

void NaivePathIntegrator::traceRadiance(
    const Scene&    scene, 
    const Ray&      ray,
    Spectrum* const out_radiance) const {

    CADISE_ASSERT(out_radiance);

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

        const Vector3R& P  = intersection.surfaceInfo().point();
        const Vector3R& Ns = intersection.surfaceInfo().shadingNormal();

        const AreaLight* areaLight = primitive->areaLight();
        if (areaLight) {
            const Spectrum emittance = areaLight->emittance(intersection);
            totalRadiance += pathWeight * emittance;
        }

        const Spectrum  reflectance = bsdf->evaluateSample(TransportInfo(), intersection);
        const Vector3R& L = intersection.wo();

        const real LdotN = L.absDot(Ns);
        pathWeight *= reflectance * LdotN / intersection.pdf();

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