#include "Core/Renderer/BDPT/Subpath.h"

#include "Core/Camera/Camera.h"
#include "Core/Gear/ConnectEvent.h"
#include "Core/Gear/Sample/CameraSample.h"
#include "Core/Gear/Sample/DirectLightSample.h"
#include "Core/Light/Light.h"
#include "Core/Ray.h"
#include "Core/Renderer/BDPT/BDPTMIS.h"
#include "Core/Scene.h"
#include "Core/Surface/ETransportMode.h"
#include "Foundation/Assertion.h"
#include "Math/Constant.h"

namespace cadise
{

Subpath Subpath::emptyPath()
{
    return Subpath(0);
}

Subpath Subpath::oneVertexPath(const PathVertex& vertex)
{
    Subpath path(1);
    path.addVertex(vertex);

    return path;
}

Subpath::Subpath(const std::size_t maxPathLength) :
    _vertices()
{
    _vertices.reserve(maxPathLength);
}

PathVertex& Subpath::operator[] (const std::size_t index)
{
    return _vertices[index];
}

const PathVertex& Subpath::operator[] (const std::size_t index) const
{
    return _vertices[index];
}

void Subpath::addVertex(const PathVertex& vertex)
{
    // we take _vertices as a fixed size array,
    // so it needs to check the number of elements
    // is less than the capacity.
    CS_ASSERT_LT(_vertices.size(), _vertices.capacity());

    _vertices.push_back(vertex);
}

void Subpath::connectCamera(
    const Scene&                     scene,
    const Camera* const              camera,
    std::vector<ConnectEvent>* const out_events) const
{
    CS_ASSERT(camera);
    CS_ASSERT(out_events);

    const std::size_t pathLength = this->length();
    if (pathLength < 2)
    {
        return;
    }

    CS_ASSERT_EQ(_vertices[0].type(), EVertexType::LightEnd);

    for (std::size_t s = 2; s <= pathLength; ++s)
    {
        const PathVertex& lightEndpoint = _vertices[s - 1];
        if (!lightEndpoint.isConnectible())
        {
            continue;
        }

        const Vector3R& lightP  = lightEndpoint.surfaceDetail().position();
        const Vector3R& lightNs = lightEndpoint.surfaceDetail().shadingNormal();

        Ray toCameraRay;
        CameraSample cameraSample;
        cameraSample.setTargetPosition(lightP);

        camera->evaluateCameraSample(&cameraSample, &toCameraRay);
        if (!cameraSample.isValid())
        {
            continue;
        }

        const Vector3R& cameraP           = cameraSample.cameraPosition();
        const Vector3R& cameraN           = cameraSample.cameraNormal();
        const Vector2D& filmPosition      = cameraSample.filmPosition();
        const Spectrum& importance        = cameraSample.importance();
        const real      pdfW              = cameraSample.pdfW();
        const real      distance2         = lightP.sub(cameraP).lengthSquared();
        const real      cameraToLightDotN = toCameraRay.direction().negate().absDot(cameraN);

        SurfaceDetail surfaceDetail;
        surfaceDetail.setPosition(cameraP);
        surfaceDetail.setGeometryNormal(cameraN);
        surfaceDetail.setShadingNormal(cameraN);

        PathVertex cameraVertex(EVertexType::CameraEnd, importance.div(pdfW));
        cameraVertex.setSurfaceDetail(surfaceDetail);
        cameraVertex.setCamera(camera);
        cameraVertex.setPdfAForward(cameraVertex.evaluateOriginPdfA(scene, lightEndpoint));

        const Spectrum& throughputA = cameraVertex.throughput();
        const Spectrum& throughputB = lightEndpoint.throughput();
        const Spectrum  reflectance = lightEndpoint.evaluate(ETransportMode::Importance, _vertices[s - 2], cameraVertex);
        const real      LdotN       = toCameraRay.direction().absDot(lightNs);

        Spectrum radiance = throughputA.mul(reflectance.mul(throughputB.mul(LdotN)));
        if (radiance.isZero())
        {
            continue;
        }

        // visibility test
        if (scene.isOccluded(toCameraRay))
        {
            continue;
        }

        const real misWeight = BDPTMIS::weight(
            scene,
            *this,
            Subpath::oneVertexPath(cameraVertex),
            s,
            1);

        out_events->push_back(ConnectEvent(filmPosition, radiance.mul(misWeight)));
    }
}

void Subpath::connectLight(
    const Scene&    scene,
    Spectrum* const out_radiance) const
{
    CS_ASSERT(out_radiance);

    const std::size_t pathLength = this->length();
    if (pathLength < 2)
    {
        return;
    }

    CS_ASSERT_EQ(_vertices[0].type(), EVertexType::CameraEnd);

    Spectrum totalRadiance(0.0_r);
    for (std::size_t t = 2; t <= pathLength; ++t)
    {
        const PathVertex& cameraEndpoint = _vertices[t - 1];
        if (!cameraEndpoint.isConnectible())
        {
            continue;
        }

        const Vector3R& cameraP  = cameraEndpoint.surfaceDetail().position();
        const Vector3R& cameraNs = cameraEndpoint.surfaceDetail().shadingNormal();

        real pickLightPdf;
        const Light* sampleLight = scene.sampleOneLight(&pickLightPdf);

        CS_ASSERT(sampleLight);

        DirectLightSample directLightSample;
        directLightSample.setTargetPosition(cameraP);

        sampleLight->evaluateDirectSample(&directLightSample);
        if (!directLightSample.isValid())
        {
            continue;
        }

        const Vector3R& emitP    = directLightSample.emitPosition();
        const Vector3R& emitN    = directLightSample.emitNormal();
        const Spectrum& radiance = directLightSample.radiance();
        const real      pdfW     = directLightSample.pdfW();
        const Vector3R  LVector  = emitP.sub(cameraP);
        const real      distance = LVector.length();

        CS_ASSERT_GT(distance, 0.0_r);

        const Vector3R L     = LVector.div(distance);
        const real     LdotN = L.absDot(cameraNs);

        SurfaceDetail surfaceDetail;
        surfaceDetail.setPosition(emitP);
        // TODO: refactor here
        surfaceDetail.setGeometryNormal(emitN);
        surfaceDetail.setShadingNormal(emitN);

        PathVertex lightVertex(EVertexType::LightEnd, radiance.div(pickLightPdf * pdfW));
        lightVertex.setSurfaceDetail(surfaceDetail);
        lightVertex.setLight(sampleLight);
        lightVertex.setPdfAForward(lightVertex.evaluateOriginPdfA(scene, cameraEndpoint));

        const Spectrum& throughputA = lightVertex.throughput();
        const Spectrum& throughputB = cameraEndpoint.throughput();
        const Spectrum  reflectance = cameraEndpoint.evaluate(ETransportMode::Radiance, _vertices[t - 2], lightVertex);

        const Spectrum contributeRadiance = throughputA.mul(reflectance.mul(throughputB.mul(LdotN)));
        if (contributeRadiance.isZero())
        {
            continue;
        }

        // do visibility test only when radiance is valid
        Ray testRay(cameraP, L);
        testRay.setMaxT(distance - constant::ray_epsilon<real>);

        if (scene.isOccluded(testRay))
        {
            continue;
        }

        const real misWeight = BDPTMIS::weight(
            scene,
            Subpath::oneVertexPath(lightVertex),
            *this,
            1,
            t);

        totalRadiance.addLocal(contributeRadiance.mul(misWeight));
    }

    out_radiance->set(totalRadiance);
}

std::size_t Subpath::length() const
{
    return _vertices.size();
}

} // namespace cadise