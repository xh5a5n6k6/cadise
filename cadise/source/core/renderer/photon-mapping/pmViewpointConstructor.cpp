#include "core/renderer/photon-mapping/pmViewpointConstructor.h"

#include "core/camera/camera.h"
#include "core/integral-tool/russianRoulette.h"
#include "core/integral-tool/sample/bsdfSample.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/category/areaLight.h"
#include "core/ray.h"
#include "core/renderer/photon-mapping/pmViewpoint.h"
#include "core/renderer/photon-mapping/pmViewpointSampleRay.h"
#include "core/scene.h"
#include "core/surface/bsdf/bsdf.h"
#include "core/surface/transportInfo.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"

namespace cadise 
{

PmViewpointConstructor::PmViewpointConstructor(
    const Scene* const  scene,
    const Camera* const camera,
    const real          initialRadius) :

    _scene(scene),
    _camera(camera),
    _initialRadius(initialRadius) 
{
    CS_ASSERT(scene);
    CS_ASSERT(camera);
    CS_ASSERT_GT(initialRadius, 0.0_r);
}

void PmViewpointConstructor::construct(
    const Vector2D&                 filmPosition,
    std::vector<PmViewpoint>* const out_viewpoints) const
{
    CS_ASSERT(out_viewpoints);

    const int32 MAX_PATH_LENGTH = 6;

    std::size_t   sampleViewpointCount = 0;
    TransportInfo transportInfo(ETransportMode::RADIANCE);

    Ray traceRay;
    _camera->spawnPrimaryRay(filmPosition, &traceRay);

    PmViewpointSampleRay rayBufferStack[64];
    PmViewpointSampleRay currentSampleRay = { traceRay, Spectrum(1.0_r), 0 };
    uint8                currentStackSize = 0;

    while (true) 
    {
        traceRay = currentSampleRay.sampleRay();

        SurfaceIntersection si;
        if (!_scene->isIntersecting(traceRay, si)) 
        {
            if (currentStackSize == 0) 
            {
                break;
            }
            else
            {
                --currentStackSize;
                currentSampleRay = rayBufferStack[currentStackSize];

                continue;
            }
        }

        const Primitive* primitive = si.primitiveInfo().primitive();
        const Bsdf*      bsdf      = primitive->bsdf();

        const Vector3R& P  = si.surfaceDetail().position();
        const Vector3R& Ns = si.surfaceDetail().shadingNormal();

        // build viewpoint at non-specular surface
        if (bsdf->lobes().hasAtLeastOne({
            ELobe::DIFFUSE_REFLECTION,
            ELobe::DIFFUSE_TRANSMISSION,
            ELobe::GLOSSY_REFLECTION,
            ELobe::GLOSSY_TRANSMISSION })) 
        {
            PmViewpoint viewpoint(
                bsdf,
                si.surfaceDetail(),
                si.wi(),
                filmPosition,
                currentSampleRay.pathThroughput(),
                _initialRadius);

            if (primitive->areaLight()) 
            {
                const Spectrum emittedRadiance = primitive->areaLight()->emittance(si);

                viewpoint.setEmittedRadiance(emittedRadiance);
            }

            out_viewpoints->push_back(std::move(viewpoint));

            ++sampleViewpointCount;
        }

        // recursive trace for specular component
        if ((currentSampleRay.depth() < MAX_PATH_LENGTH - 1) &&
            bsdf->lobes().hasAtLeastOne({
                ELobe::SPECULAR_REFLECTION,
                ELobe::SPECULAR_TRANSMISSION })) 
        {
            for (BsdfComponents i = 0; i < bsdf->components(); ++i) 
            {
                if (BsdfLobes({ bsdf->lobe(i) }).hasNone({
                    ELobe::SPECULAR_REFLECTION,
                    ELobe::SPECULAR_TRANSMISSION }))
                {
                    continue;
                }

                transportInfo.setComponents(i);

                BsdfSample bsdfSample;
                bsdf->evaluateSample(transportInfo, si, &bsdfSample);
                if (!bsdfSample.isValid()) 
                {
                    continue;
                }

                const Spectrum& reflectance = bsdfSample.scatterValue();
                const Vector3R& L           = bsdfSample.scatterDirection();
                const real      pdfW        = bsdfSample.pdfW();
                const real      LdotN       = L.absDot(Ns);

                const Spectrum newPathThroughput 
                    = currentSampleRay.pathThroughput().mul(reflectance).mul(LdotN / pdfW);

                rayBufferStack[currentStackSize] = { Ray(P, L), newPathThroughput, currentSampleRay.depth() + 1 };
                ++currentStackSize;
            }
        }

        // check if there is any untraced sample
        if (currentStackSize == 0) 
        {
            break;
        }
        else 
        {
            --currentStackSize;
            currentSampleRay = rayBufferStack[currentStackSize];

            continue;
        }
    } // end while loop

    if (sampleViewpointCount > 0) 
    {
        const std::size_t numViewpoints            = out_viewpoints->size();
        const real        realSampleViewpointCount = static_cast<real>(sampleViewpointCount);

        for (std::size_t i = numViewpoints - sampleViewpointCount; i < numViewpoints; ++i) 
        {
            const Spectrum& unnormalizeThroughput = (*out_viewpoints)[i].throughputImportance();

            (*out_viewpoints)[i].setThroughputImportance(
                unnormalizeThroughput.mul(realSampleViewpointCount));
        }
    }
    else 
    {
        // add zero contribution viewpoint

        PmViewpoint viewpoint(
            nullptr,
            SurfaceDetail(),
            Vector3R(),
            filmPosition,
            Spectrum(),
            0.0_r
        );

        out_viewpoints->push_back(std::move(viewpoint));
    }
}

} // namespace cadise