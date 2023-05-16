#include "Core/Renderer/PhotonMapping/PMViewpointConstructor.h"

#include "Core/Camera/Camera.h"
#include "Core/Gear/Sample/BSDFSample.h"
#include "Core/Intersector/Primitive/Primitive.h"
#include "Core/Light/AreaLight.h"
#include "Core/Ray.h"
#include "Core/Renderer/PhotonMapping/PMViewpoint.h"
#include "Core/Renderer/PhotonMapping/PMViewpointSampleRay.h"
#include "Core/Scene.h"
#include "Core/Surface/BSDF/BSDF.h"
#include "Core/Surface/TransportInfo.h"
#include "Core/SurfaceIntersection.h"
#include "Foundation/Assertion.h"

namespace cadise
{

PMViewpointConstructor::PMViewpointConstructor(
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

void PMViewpointConstructor::construct(
    const Vector2D&                 filmPosition,
    std::vector<PMViewpoint>* const out_viewpoints) const
{
    CS_ASSERT(out_viewpoints);

    const int32 MAX_PATH_LENGTH = 6;

    std::size_t   sampleViewpointCount = 0;
    TransportInfo transportInfo(ETransportMode::Radiance);

    Ray traceRay;
    _camera->spawnPrimaryRay(filmPosition, &traceRay);

    PMViewpointSampleRay rayBufferStack[64];
    PMViewpointSampleRay currentSampleRay = { traceRay, Spectrum(1.0_r), 0 };
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
        const BSDF*      bsdf      = primitive->bsdf();

        const Vector3R& P  = si.surfaceDetail().position();
        const Vector3R& Ns = si.surfaceDetail().shadingNormal();

        // build viewpoint at non-specular surface
        if (bsdf->lobes().hasAtLeastOne({
            ELobe::DiffuseReflection,
            ELobe::DiffuseTransmission,
            ELobe::GlossyReflection,
            ELobe::GlossyTransmission }))
        {
            PMViewpoint viewpoint(
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
                ELobe::SpecularReflection,
                ELobe::SpecularTransmission }))
        {
            for (BSDFComponents i = 0; i < bsdf->components(); ++i)
            {
                if (BSDFLobes({ bsdf->lobe(i) }).hasNone({
                    ELobe::SpecularReflection,
                    ELobe::SpecularTransmission }))
                {
                    continue;
                }

                transportInfo.setComponents(i);

                BSDFSample bsdfSample;
                bsdf->evaluateSample(transportInfo, si, &bsdfSample);
                if (!bsdfSample.isValid())
                {
                    continue;
                }

                const Spectrum& reflectance = bsdfSample.scatterValue();
                const Vector3R& L           = bsdfSample.scatterDirection();
                const real      pdfW        = bsdfSample.pdfW();
                const real      LdotN       = L.absDot(Ns);

                const Spectrum newPathThroughput = currentSampleRay.pathThroughput()
                    .mul(reflectance)
                    .mul(LdotN / pdfW);

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
            const Spectrum& unnormalizedThroughput = (*out_viewpoints)[i].throughputImportance();

            (*out_viewpoints)[i].setThroughputImportance(
                unnormalizedThroughput.mul(realSampleViewpointCount));
        }
    }
    else
    {
        // add zero contribution viewpoint

        PMViewpoint viewpoint(
            nullptr,
            SurfaceDetail(),
            Vector3R(),
            filmPosition,
            Spectrum(),
            0.0_r);

        out_viewpoints->push_back(std::move(viewpoint));
    }
}

} // namespace cadise