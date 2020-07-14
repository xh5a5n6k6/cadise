#include "core/integrator/naivePathIntegrator.h"

#include "core/integral-tool/russianRoulette.h"
#include "core/integral-tool/sample/bsdfSample.h"
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

    const TransportInfo transportInfo(ETransportMode::RADIANCE);
    
    Spectrum totalRadiance(0.0_r);
    Spectrum pathThroughput(1.0_r);
    Ray      traceRay(ray);

    for (int32 bounceTimes = 0; bounceTimes < _maxDepth; ++bounceTimes) {
        SurfaceIntersection intersection;
        if (!scene.isIntersecting(traceRay, intersection)) {
            break;
        }

        const Primitive* primitive = intersection.primitiveInfo().primitive();
        const Bsdf*      bsdf      = primitive->bsdf();

        const Vector3R& P  = intersection.surfaceDetail().position();
        const Vector3R& Ns = intersection.surfaceDetail().shadingNormal();

        const AreaLight* areaLight = primitive->areaLight();
        if (areaLight) {
            const Spectrum emittance = areaLight->emittance(intersection);
            totalRadiance += pathThroughput * emittance;
        }

        BsdfSample bsdfSample;
        bsdf->evaluateSample(transportInfo, intersection, &bsdfSample);
        if (!bsdfSample.isValid()) {
            break;
        }

        const Spectrum& reflectance = bsdfSample.scatterValue();
        const Vector3R& L           = bsdfSample.scatterDirection();
        const real      pdfW        = bsdfSample.pdfW();
        const real      LdotN       = L.absDot(Ns);

        pathThroughput *= reflectance * LdotN / pdfW;

        // use russian roulette to decide if the ray needs to be kept tracking
        if (bounceTimes > 2) {
            Spectrum newPathThroughput;
            if (!RussianRoulette::isSurvivedOnNextRound(pathThroughput, &newPathThroughput)) {
                break;
            }

            pathThroughput = newPathThroughput;
        }

        if (pathThroughput.isZero()) {
            break;
        }

        traceRay.reset();
        traceRay.setOrigin(P);
        traceRay.setDirection(L);
    }
    
    *out_radiance = totalRadiance;
}

} // namespace cadise