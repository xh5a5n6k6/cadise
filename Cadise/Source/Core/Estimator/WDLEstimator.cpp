#include "Core/Estimator/WDLEstimator.h"

#include "Core/Gear/Sample/BSDFSample.h"
#include "Core/Gear/Sample/DirectLightSample.h"
#include "Core/Intersector/Primitive/Primitive.h"
#include "Core/Light/AreaLight.h"
#include "Core/Ray.h"
#include "Core/Scene.h"
#include "Core/Surface/BSDF/BSDF.h"
#include "Core/Surface/TransportInfo.h"
#include "Core/SurfaceIntersection.h"
#include "Foundation/Assertion.h"
#include "Math/Constant.h"

namespace cadise
{

WDLEstimator::WDLEstimator(const int32 maxDepth) :
    RadianceEstimator(),
    _maxDepth(maxDepth)
{
    CS_ASSERT_GE(maxDepth, 0);
}

void WDLEstimator::estimate(
    const Scene&    scene,
    const Ray&      ray,
    Spectrum* const out_radiance) const
{
    CS_ASSERT(out_radiance);

    const TransportInfo transportInfo(ETransportMode::Radiance);

    Spectrum totalRadiance(0.0_r);
    Spectrum pathThroughput(1.0_r);
    Ray      traceRay(ray);

    for (int32 bounceTimes = 0; bounceTimes < _maxDepth; ++bounceTimes)
    {
        SurfaceIntersection intersection;
        if (!scene.isIntersecting(traceRay, intersection))
        {
            break;
        }

        const Primitive* primitive = intersection.primitiveInfo().primitive();
        const BSDF*      bsdf      = primitive->bsdf();

        const Vector3R& P  = intersection.surfaceDetail().position();
        const Vector3R& Ns = intersection.surfaceDetail().shadingNormal();

        const bool isSpecular = bsdf->lobes().hasAtLeastOne({
            ELobe::SpecularReflection,
            ELobe::SpecularTransmission });

        // add radiance if hitting area light
        if (primitive->areaLight())
        {
            const Spectrum emittance = primitive->areaLight()->emittance(intersection);

            totalRadiance.addLocal(pathThroughput.mul(emittance));
        }

        // add direct light only at non-specular surface
        if (!isSpecular)
        {
            real lightPdf;
            const Light* sampleLight = scene.sampleOneLight(&lightPdf);

            DirectLightSample directLightSample;
            directLightSample.setTargetPosition(P);

            sampleLight->evaluateDirectSample(&directLightSample);
            if (directLightSample.isValid())
            {
                const Vector3R LVector  = directLightSample.emitPosition().sub(P);
                const real     distance = LVector.length();

                CS_ASSERT_GT(distance, 0.0_r);

                const Vector3R L = LVector.div(distance);
                intersection.setWo(L);

                // generate shadow ray to do occluded test
                Ray shadowRay(P, L);
                shadowRay.setMaxT(distance - constant::ray_epsilon<real>);

                if (!scene.isOccluded(shadowRay))
                {
                    const Spectrum& radiance  = directLightSample.radiance();
                    const real      lightPdfW = directLightSample.pdfW();

                    const Spectrum reflectance         = bsdf->evaluate(TransportInfo(), intersection);
                    const Spectrum directLightFactor   = reflectance.mul(L.absDot(Ns) / lightPdfW);
                    const Spectrum directLightRadiance = radiance.mul(directLightFactor);

                    totalRadiance.addLocal(pathThroughput.mul(directLightRadiance.div(lightPdf)));
                }
            }

            // once hitting non-specular surface, we
            // calculate direct lighting and then exit
            break;
        }

        // only trace next ray at specular surface
        // according to bsdf sampling
        BSDFSample bsdfSample;
        bsdf->evaluateSample(transportInfo, intersection, &bsdfSample);
        if (!bsdfSample.isValid())
        {
            break;
        }

        const Spectrum& reflectance = bsdfSample.scatterValue();
        const Vector3R& L           = bsdfSample.scatterDirection();
        const real      pdfW        = bsdfSample.pdfW();
        const real      LdotN       = L.absDot(Ns);

        pathThroughput.mulLocal(reflectance.mul(LdotN / pdfW));
        if (pathThroughput.isZero())
        {
            break;
        }

        traceRay.reset();
        traceRay.setOrigin(P);
        traceRay.setDirection(L);
    }

    out_radiance->set(totalRadiance);
}

} // namespace cadise