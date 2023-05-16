#include "Core/Renderer/BDPT/SubpathBuilder.h"

#include "Core/Camera/Camera.h"
#include "Core/Gear/RussianRoulette.h"
#include "Core/Gear/Sample/BSDFSample.h"
#include "Core/Gear/Sample/EmitLightSample.h"
#include "Core/Intersector/Primitive/Primitive.h"
#include "Core/Light/AreaLight.h"
#include "Core/Ray.h"
#include "Core/Renderer/BDPT/BDPTMIS.h"
#include "Core/Renderer/BDPT/Subpath.h"
#include "Core/Scene.h"
#include "Core/Surface/BSDF/BSDF.h"
#include "Core/Surface/TransportInfo.h"
#include "Core/SurfaceIntersection.h"
#include "Foundation/Assertion.h"

namespace cadise
{

SubpathBuilder::SubpathBuilder(const std::size_t maxPathLength) :
    _maxPathLength(maxPathLength),
    _camera(nullptr)
{}

void SubpathBuilder::setCamera(const Camera* const camera)
{
    CS_ASSERT(camera);

    _camera = camera;
}

void SubpathBuilder::buildLightPath(
    const Scene&   scene,
    Subpath* const out_lightPath) const
{
    CS_ASSERT(out_lightPath);

    real pickLightPdf;
    const Light* sampleLight = scene.sampleOneLight(&pickLightPdf);

    CS_ASSERT(sampleLight);

    EmitLightSample emitLightSample;
    sampleLight->evaluateEmitSample(&emitLightSample);
    if (!emitLightSample.isValid())
    {
        return;
    }

    const Spectrum& emittance     = emitLightSample.radiance();
    const Vector3R& emitPosition  = emitLightSample.emitPosition();
    const Vector3R& emitDirection = emitLightSample.emitDirection();
    const Vector3R& emitN         = emitLightSample.emitNormal();
    const real      emitPdfA      = emitLightSample.pdfA();
    const real      emitPdfW      = emitLightSample.pdfW();
    const real      pdfAForward   = pickLightPdf * emitPdfA;

    SurfaceDetail surfaceDetail;
    surfaceDetail.setPosition(emitPosition);
    surfaceDetail.setGeometryNormal(emitN);
    surfaceDetail.setShadingNormal(emitN);

    PathVertex lightVertex(EVertexType::LightEnd, emittance);
    lightVertex.setSurfaceDetail(surfaceDetail);
    lightVertex.setPdfAForward(pdfAForward);
    lightVertex.setLight(sampleLight);

    out_lightPath->addVertex(lightVertex);

    const real     numerator   = emitDirection.absDot(emitN);
    const real     denominator = pickLightPdf * emitPdfA * emitPdfW;
    const Spectrum throughput  = emittance.mul(numerator / denominator);

    // it will not used in light sub-path construction
    Spectrum localRadiance;
    _buildSubpathCompletely(
        ETransportMode::Importance,
        scene,
        Ray(emitPosition, emitDirection),
        throughput,
        emitPdfW,
        out_lightPath,
        &localRadiance);
}

void SubpathBuilder::buildCameraPath(
    const Scene&    scene,
    const Vector2D& filmPosition,
    Subpath* const  out_cameraPath,
    Spectrum* const out_zeroBounceRadiance) const
{
    CS_ASSERT(out_cameraPath);
    CS_ASSERT(out_zeroBounceRadiance);
    CS_ASSERT(_camera);

    Ray primaryRay;
    _camera->spawnPrimaryRay(filmPosition, &primaryRay);

    real pdfA;
    real pdfW;
    _camera->evaluateCameraPdf(primaryRay, &pdfA, &pdfW);

    SurfaceDetail surfaceDetail;
    surfaceDetail.setPosition(primaryRay.origin());
    // TODO: refactor here
    //const Vector3R cameraN(0.0_r, 0.0_r, -1.0_r);
    surfaceDetail.setGeometryNormal(primaryRay.direction());
    surfaceDetail.setShadingNormal(primaryRay.direction());

    PathVertex cameraVertex(EVertexType::CameraEnd, Spectrum(1.0_r));
    cameraVertex.setSurfaceDetail(surfaceDetail);
    cameraVertex.setPdfAForward(pdfA);
    cameraVertex.setCamera(_camera);

    out_cameraPath->addVertex(cameraVertex);

    _buildSubpathCompletely(
        ETransportMode::Radiance,
        scene,
        primaryRay,
        Spectrum(1.0_r),
        pdfW,
        out_cameraPath,
        out_zeroBounceRadiance);
}

void SubpathBuilder::_buildSubpathCompletely(
    const ETransportMode mode,
    const Scene&         scene,
    const Ray&           firstRay,
    const Spectrum&      secondVertexThroughput,
    const real           secondVertexForwardPdfW,
    Subpath* const       out_subpath,
    Spectrum* const      out_zeroBounceRadiance) const
{
    CS_ASSERT(out_subpath);
    CS_ASSERT(out_zeroBounceRadiance);

    std::size_t currentLength = out_subpath->length();
    if (currentLength == _maxPathLength)
    {
        return;
    }

    Ray      traceRay(firstRay);
    Spectrum throughput  = secondVertexThroughput;
    real     pdfWForward = secondVertexForwardPdfW;
    real     pdfWReverse = 0.0_r;

    for (int32 bounceTimes = 0; bounceTimes <= _maxPathLength; ++bounceTimes)
    {
        SurfaceIntersection intersection;
        if (!scene.isIntersecting(traceRay, intersection))
        {
            break;
        }

        PathVertex& previousVertex = (*out_subpath)[currentLength - 1];
        PathVertex  newVertex(EVertexType::Surface, throughput);

        const Primitive* primitive = intersection.primitiveInfo().primitive();
        const BSDF*      bsdf      = primitive->bsdf();

        const Vector3R& newP       = intersection.surfaceDetail().position();
        const Vector3R& newNs      = intersection.surfaceDetail().shadingNormal();
        const Vector3R& previousP  = previousVertex.surfaceDetail().position();
        const Vector3R& previousNs = previousVertex.surfaceDetail().shadingNormal();

        const real distance          = traceRay.maxT();
        const real distance2         = distance * distance;
        const real previousToNewDotN = traceRay.direction().absDot(previousNs);
        const real newToPreviousDotN = traceRay.direction().negate().absDot(newNs);

        newVertex.setSurfaceDetail(intersection.surfaceDetail());
        newVertex.setPdfAForward(pdfWForward * newToPreviousDotN / distance2);
        newVertex.setBsdf(bsdf);

        out_subpath->addVertex(newVertex);
        ++currentLength;

        // add s=0 situation radiance when hitting area light
        // (while building camera sub-path)
        const AreaLight* areaLight = primitive->areaLight();
        if (areaLight && mode == ETransportMode::Radiance)
        {
            (*out_subpath)[currentLength - 1].setLight(areaLight);

            const Spectrum emittance = areaLight->emittance(intersection);
            const real     misWeight = BDPTMIS::weight(scene, Subpath::emptyPath(), *out_subpath, 0, currentLength);

            out_zeroBounceRadiance->addLocal(throughput.mul(emittance.mul(misWeight)));
        }

        if (currentLength == _maxPathLength)
        {
            break;
        }

        // estimate next direction with bsdf sampling
        BSDFSample bsdfSample;
        bsdf->evaluateSample(TransportInfo(mode), intersection, &bsdfSample);
        if (!bsdfSample.isValid())
        {
            break;
        }

        const Spectrum& reflectance = bsdfSample.scatterValue();
        const Vector3R& L           = bsdfSample.scatterDirection();
        const real      pdfW        = bsdfSample.pdfW();
        const real      LdotN       = L.absDot(newNs);

        intersection.setWo(L);

        // for non-symmetric scattering correction
        throughput.mulLocal(reflectance.mul(LdotN / pdfW));
        if (mode == ETransportMode::Importance)
        {
            throughput.mulLocal(1.0_r);
        }

        pdfWForward = pdfW;
        pdfWReverse = bsdf->evaluatePdfW(TransportInfo(mode), intersection.reverse());

        // for specular surface, it is impossible
        // to connect from both sides
        if (!newVertex.isConnectible())
        {
            pdfWForward = 0.0_r;
            pdfWReverse = 0.0_r;
        }

        real pdfAReverse = pdfWReverse / distance2;
        if (previousVertex.camera() == nullptr)
        {
            pdfAReverse *= previousToNewDotN;
        }
        previousVertex.setPdfAReverse(pdfAReverse);

        // use russian roulette to decide if the ray needs to be kept tracking
        if (bounceTimes > 10)
        {
            Spectrum newThroughput;
            if (!RussianRoulette::isSurvivedOnNextRound(throughput, &newThroughput))
            {
                break;
            }

            throughput = newThroughput;
        }

        if (throughput.isZero())
        {
            break;
        }

        traceRay.reset();
        traceRay.setOrigin(newP);
        traceRay.setDirection(L);
    } // end for loop
}

} // namespace cadise