#include "core/estimator/ptEstimator.h"

#include "core/integral-tool/russianRoulette.h"
#include "core/integral-tool/sample/bsdfSample.h"
#include "core/integral-tool/sample/directLightSample.h"
#include "core/integral-tool/tMis.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/category/areaLight.h"
#include "core/ray.h"
#include "core/scene.h"
#include "core/surface/bsdf/bsdf.h"
#include "core/surface/transportInfo.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"
#include "fundamental/logger/logger.h"
#include "math/constant.h"

namespace cadise 
{

namespace
{
    const Logger logger("PT Estimator");
}

PtEstimator::PtEstimator(const int32 maxPathLength) :
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

void PtEstimator::estimate(
    const Scene&    scene, 
    const Ray&      ray,
    Spectrum* const out_radiance) const 
{
    CS_ASSERT(out_radiance);

    const TransportInfo transportInfo(ETransportMode::Radiance);

    SurfaceIntersection si;
    Spectrum            totalRadiance(0.0_r);
    Spectrum            pathThroughput(1.0_r);
    Ray                 traceRay(ray);

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
        const Bsdf*      bsdf      = primitive->bsdf();

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
                            const real misWeight = TMis<EMisMode::Power>().weight(totalLightPdfW, bsdfPdfW);

                            misLightRadiance = radiance.mul(factor).mul(misWeight);
                            totalRadiance.addLocal(pathThroughput.mul(misLightRadiance));
                        }
                    }
                }
            }
        } // end light sampling

        // sampling from bsdf
        {
            BsdfSample bsdfSample;
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
                    const real misWeight      = TMis<EMisMode::Power>().weight(bsdfPdfW, totalLightPdfW);

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