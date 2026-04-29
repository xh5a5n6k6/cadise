#include "Core/Estimator/PTEstimator.h"

#include "Core/Gear/RussianRoulette.h"
#include "Core/Gear/Sample/BSDFSample.h"
#include "Core/Gear/Sample/DirectLightSample.h"
#include "Core/Gear/TMIS.h"
#include "Core/Intersector/Primitive/Primitive.h"
#include "Core/Light/AreaLight.h"
#include "Core/Ray.h"
#include "Core/Scene.h"
#include "Core/Surface/BSDF/BSDF.h"
#include "Core/Surface/TransportInfo.h"
#include "Core/SurfaceIntersection.h"
#include "Foundation/Assertion.h"
#include "Foundation/Logging/Logger.h"
#include "Math/Constant.h"

namespace cadise
{

namespace
{
    const Logger logger("PT Estimator");
}

PTEstimator::PTEstimator(const int32 maxPathLength) :
    RadianceEstimator(),
    _maxPathLength(maxPathLength)
{
    if (_maxPathLength <= 0)
    {
        logger.log(ELogLevel::Warn,
            "Invalid value on <maxPathLength>: " + std::to_string(maxPathLength) +
            ", it should be greater than 0, set fallback value to 1");

        _maxPathLength = 1;
    }
}

void PTEstimator::estimate(
    const Scene&    scene,
    const Ray&      ray,
    Spectrum* const out_radiance) const
{
    CS_ASSERT(out_radiance);

    const TransportInfo         transportInfo(ETransportMode::Radiance);
    const TMIS<EMISMode::Power> mis;

    SurfaceIntersection   si;
    Spectrum              totalRadiance(0.0_r);
    Spectrum              pathThroughput(1.0_r);
    Ray                   traceRay(ray);

    if (!scene.isIntersecting(traceRay, si))
    {
        out_radiance->set(totalRadiance);

        return;
    }

    // 0-bounce radiance
    {
        const Primitive* prim = si.primitiveInfo().primitive();
        if (prim->areaLight())
        {
            const Spectrum emittance = prim->areaLight()->emittance(si);

            totalRadiance.addLocal(pathThroughput.mul(emittance));
        }
    }

    // bounce time = path length - 1
    // 
    // ex. p_0 ---> p_1 ---> p_2, 
    //     bounce time = 1
    //     path length = 2
    for (int32 bounceTimes = 1; bounceTimes < _maxPathLength; ++bounceTimes)
    {
        const Primitive* primitive = si.primitiveInfo().primitive();
        const BSDF*      bsdf      = primitive->bsdf();

        const Vector3R& P  = si.surfaceDetail().position();
        const Vector3R& Ns = si.surfaceDetail().shadingNormal();

        // early break if hitting envrionment light
        if (bsdf->lobes().hasExactly({ ELobe::Absorb }))
        {
            break;
        }

        // TODO: it should be decided according to sampled bxdf
        bool canDoMis = false;
        if (!bsdf->lobes().hasAtLeastOne({
            ELobe::SpecularReflection,
            ELobe::SpecularTransmission }))
        {
            canDoMis = true;
        }

        // flag used in bsdf sampling
        bool isSampledDeltaLight = false;

        // sampling from light
        {
            if (canDoMis)
            {
                real sampleLightPdf;
                const Light* sampleLight = scene.sampleOneLight(&sampleLightPdf);

                CS_ASSERT(sampleLight);

                DirectLightSample directLightSample;
                directLightSample.setTargetPosition(P);

                sampleLight->evaluateDirectSample(&directLightSample);
                if (directLightSample.isValid())
                {
                    const Vector3R LVector  = directLightSample.emitPosition().sub(P);
                    const real     distance = LVector.length();

                    CS_ASSERT_GT(distance, 0.0_r);

                    const Vector3R L = LVector.div(distance);
                    si.setWo(L);

                    // generate shadow ray to do occluded test
                    Ray shadowRay(P, L);
                    shadowRay.setMaxT(distance - constant::ray_epsilon<real>);

                    if (!scene.isOccluded(shadowRay))
                    {
                        const Spectrum& radiance       = directLightSample.radiance();
                        const real      lightPdfW      = directLightSample.pdfW();
                        const real      totalLightPdfW = sampleLightPdf * lightPdfW;

                        const Spectrum reflectance = bsdf->evaluate(transportInfo, si);
                        const Spectrum factor      = reflectance.mul(L.absDot(Ns) / totalLightPdfW);

                        // if sample light is delta light, not using mis technique in bsdf sampling
                        Spectrum misLightRadiance;
                        if (sampleLight->isDeltaLight())
                        {
                            isSampledDeltaLight = true;

                            misLightRadiance = radiance.mul(factor);
                            totalRadiance.addLocal(pathThroughput.mul(misLightRadiance));
                        }
                        else
                        {
                            const real bsdfPdfW  = bsdf->evaluatePdfW(transportInfo, si);
                            const real misWeight = mis.weight(totalLightPdfW, bsdfPdfW);

                            misLightRadiance = radiance.mul(factor).mul(misWeight);
                            totalRadiance.addLocal(pathThroughput.mul(misLightRadiance));
                        }
                    }
                }
            }
        } // end light sampling

        // sampling from bsdf
        {
            BSDFSample bsdfSample;
            bsdf->evaluateSample(transportInfo, si, &bsdfSample);
            if (!bsdfSample.isValid())
            {
                break;
            }

            const Spectrum& reflectance = bsdfSample.scatterValue();
            const Vector3R& L           = bsdfSample.scatterDirection();
            const real      bsdfPdfW    = bsdfSample.pdfW();
            const real      LdotN       = L.absDot(Ns);

            traceRay.reset();
            traceRay.setOrigin(P);
            traceRay.setDirection(L);

            SurfaceIntersection nextSi;
            if (!scene.isIntersecting(traceRay, nextSi))
            {
                break;
            }

            const Primitive* nextPrimitive = nextSi.primitiveInfo().primitive();
            if (nextPrimitive->areaLight())
            {
                const AreaLight* areaLight = nextPrimitive->areaLight();
                const Spectrum   radiance  = areaLight->emittance(nextSi);
                const Spectrum   factor    = reflectance.mul(L.absDot(Ns) / bsdfPdfW);

                Spectrum misBsdfRadiance;
                if (canDoMis && !isSampledDeltaLight)
                {
                    const real sampleLightPdf = scene.evaluatePickLightPdf(areaLight);
                    const real lightPdfW      = areaLight->evaluateDirectPdfW(nextSi, P);
                    const real totalLightPdfW = sampleLightPdf * lightPdfW;
                    const real misWeight      = mis.weight(bsdfPdfW, totalLightPdfW);

                    misBsdfRadiance = radiance.mul(factor).mul(misWeight);
                    totalRadiance.addLocal(pathThroughput.mul(misBsdfRadiance));
                }
                else if (!canDoMis)
                {
                    misBsdfRadiance = radiance.mul(factor);
                    totalRadiance.addLocal(pathThroughput.mul(misBsdfRadiance));
                }
            }

            pathThroughput.mulLocal(reflectance.mul(LdotN / bsdfPdfW));

            si = nextSi;
        } // end bsdf sampling

        // use russian roulette to decide if the ray needs to be kept tracking
        if (bounceTimes > 2)
        {
            Spectrum newPathThroughput;
            if (!RussianRoulette::isSurvivedOnNextRound(pathThroughput, &newPathThroughput))
            {
                break;
            }

            pathThroughput = newPathThroughput;
        }

        if (pathThroughput.isZero())
        {
            break;
        }
    }

    out_radiance->set(totalRadiance);
}

} // namespace cadise