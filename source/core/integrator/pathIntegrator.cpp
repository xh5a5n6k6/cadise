#include "core/integrator/pathIntegrator.h"

#include "core/integral-tool/directLightEvaluator.h"
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

PathIntegrator::PathIntegrator(const int32 maxDepth) :
    _maxDepth(maxDepth) {
}

void PathIntegrator::traceRadiance(
    const Scene&    scene, 
    const Ray&      ray,
    Spectrum* const out_radiance) const {

    CADISE_ASSERT(out_radiance);

    Spectrum totalRadiance(0.0_r);
    Spectrum pathThroughput(1.0_r);
    Ray      traceRay(ray);

    // set this flag true at 0 bounce,
    // and update it at each intersection
    // (specular surface for true, non-specular surface for false)
    bool isCountForEmittance = true;

    for (int32 bounceTimes = 0; bounceTimes < _maxDepth; ++bounceTimes) {
        SurfaceIntersection intersection;
        if (!scene.isIntersecting(traceRay, intersection)) {
            break;
        }

        const Primitive* primitive = intersection.primitiveInfo().primitive();
        const Bsdf*      bsdf      = primitive->bsdf();

        const Vector3R& P  = intersection.surfaceInfo().position();
        const Vector3R& Ns = intersection.surfaceInfo().shadingNormal();

        // add emitter's emittance only at first hit-point (0 bounce)
        // or previous hit surface is specular
        const AreaLight* areaLight = primitive->areaLight();
        if (areaLight && isCountForEmittance) {
            const Spectrum emittance = areaLight->emittance(intersection);
            totalRadiance += pathThroughput * emittance;
        }


        // estimate direct light using MIS technique 
        // (only at non-specular surface)
        // TODO: FIX HERE
        //       use sample bxdfType to do this check instead
        if (!bsdf->type().hasAtLeastOne(BxdfType::SPECULAR_REFLECTION,
                                        BxdfType::SPECULAR_TRANSMISSION)) {

            isCountForEmittance = false;

            real lightPdf;
            const Light* sampleLight = scene.sampleOneLight(&lightPdf);

            CADISE_ASSERT(sampleLight);
            CADISE_ASSERT_GT(lightPdf, 0.0_r);

            const Spectrum directLightRadiance 
                = DirectLightEvaluator::evaluate(scene, intersection, bsdf, sampleLight) / lightPdf;
            
            totalRadiance += pathThroughput * directLightRadiance;
        }
        else {
            isCountForEmittance = true;
        }

        // estimate indirect light with bsdf sampling
        BsdfSample bsdfSample;
        bsdf->evaluateSample(TransportInfo(), intersection, &bsdfSample);
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