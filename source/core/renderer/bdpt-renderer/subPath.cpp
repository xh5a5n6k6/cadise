#include "core/renderer/bdpt-renderer/subPath.h"

#include "core/camera/camera.h"
#include "core/integral-tool/connectEvent.h"
#include "core/integral-tool/sample/cameraSample.h"
#include "core/integral-tool/sample/directLightSample.h"
#include "core/light/light.h"
#include "core/ray.h"
#include "core/renderer/bdpt-renderer/bdptMis.h"
#include "core/scene.h"
#include "core/surface/transportMode.h"
#include "fundamental/assertion.h"
#include "math/constant.h"

namespace cadise {

SubPath SubPath::emptyPath() {
    return SubPath(0);
}

SubPath SubPath::oneVertexPath(const PathVertex& vertex) {
    SubPath path(1);
    path.addVertex(vertex);

    return path;
}

SubPath::SubPath(const std::size_t maxPathLength) :
    _vertices() {

    _vertices.reserve(maxPathLength);
}

PathVertex& SubPath::operator[](const std::size_t index) {
    return _vertices[index];
}

const PathVertex& SubPath::operator[](const std::size_t index) const {
    return _vertices[index];
}

void SubPath::addVertex(const PathVertex& vertex) {
    // we take _vertices as a fixed size array,
    // so it needs to check the number of elements
    // is less than the capacity.
    CADISE_ASSERT_LT(_vertices.size(), _vertices.capacity());

    _vertices.push_back(vertex);
}

void SubPath::connectCamera(
    const Scene&                     scene,
    const Camera* const              camera,
    std::vector<ConnectEvent>* const out_events) const {

    CADISE_ASSERT(out_events);

    const std::size_t pathLength = this->length();
    if (pathLength < 2) {
        return;
    }

    CADISE_ASSERT_EQ(_vertices[0].type(), VertexType::LIGHT_END);

    for (std::size_t s = 2; s <= pathLength; ++s) {
        const PathVertex& lightEndpoint = _vertices[s - 1];
        if (!lightEndpoint.isConnectible()) {
            continue;
        }

        const Vector3R& lightP  = lightEndpoint.surfaceInfo().position();
        const Vector3R& lightNs = lightEndpoint.surfaceInfo().shadingNormal();

        Ray toCameraRay;
        CameraSample cameraSample;
        cameraSample.setTargetPosition(lightP);

        camera->evaluateCameraSample(&cameraSample, &toCameraRay);
        if (!cameraSample.isValid()) {
            continue;
        }

        const Vector3R& cameraP           = cameraSample.cameraPosition();
        const Vector3R& cameraN           = cameraSample.cameraNormal();
        const Vector2R& filmNdcPosition   = cameraSample.filmNdcPosition();
        const Spectrum& importance        = cameraSample.importance();
        const real      pdfW              = cameraSample.pdfW();
        const real      distance2         = (lightP - cameraP).lengthSquared();
        const real      cameraToLightDotN = toCameraRay.direction().reverse().absDot(cameraN);

        SurfaceInfo surfaceInfo;
        surfaceInfo.setPosition(cameraP);
        surfaceInfo.setGeometryNormal(cameraN);
        surfaceInfo.setShadingNormal(cameraN);

        PathVertex cameraVertex(VertexType::CAMERA_END, importance /  pdfW);
        cameraVertex.setSurfaceInfo(surfaceInfo);
        cameraVertex.setPdfAForward(pdfW * cameraToLightDotN / distance2);
        cameraVertex.setCamera(camera);

        const Spectrum& throughputA = cameraVertex.throughput();
        const Spectrum& throughputB = lightEndpoint.throughput();
        const Spectrum  reflectance = lightEndpoint.evaluate(TransportMode::IMPORTANCE, _vertices[s - 2], cameraVertex);
        const real      LdotN       = toCameraRay.direction().absDot(lightNs);

        Spectrum radiance = throughputA * reflectance * LdotN * throughputB;
        if (radiance.isZero()) {
            continue;
        }

        // visibility test
        if (scene.isOccluded(toCameraRay)) {
            continue;
        }

        const real misWeight = BdptMis::weight(
            scene, *this, SubPath::oneVertexPath(cameraVertex), s, 1);

        out_events->push_back(ConnectEvent(filmNdcPosition, radiance * misWeight));
    }
}

void SubPath::connectLight(
    const Scene&    scene,
    Spectrum* const out_radiance) const {
    
    CADISE_ASSERT(out_radiance);

    const std::size_t pathLength = this->length();
    if (pathLength < 2) {
        return;
    }

    CADISE_ASSERT_EQ(_vertices[0].type(), VertexType::CAMERA_END);

    Spectrum totalRadiance(0.0_r);
    for (std::size_t t = 2; t <= pathLength; ++t) {
        const PathVertex& cameraEndpoint = _vertices[t - 1];
        if (!cameraEndpoint.isConnectible()) {
            continue;
        }

        const Vector3R& cameraP  = cameraEndpoint.surfaceInfo().position();
        const Vector3R& cameraNs = cameraEndpoint.surfaceInfo().shadingNormal();

        real pickLightPdf;
        const Light* sampleLight = scene.sampleOneLight(&pickLightPdf);

        CADISE_ASSERT(sampleLight);

        DirectLightSample directLightSample;
        directLightSample.setTargetPosition(cameraP);

        sampleLight->evaluateDirectSample(&directLightSample);
        if (!directLightSample.isValid()) {
            continue;
        }

        const Vector3R& emitP    = directLightSample.emitPosition();
        const Vector3R& emitN    = directLightSample.emitNormal();
        const Spectrum& radiance = directLightSample.radiance();
        const real      pdfW     = directLightSample.pdfW();
        const Vector3R  LVector  = emitP - cameraP;
        const real      distance = LVector.length();

        CADISE_ASSERT_GT(distance, 0.0_r);

        const Vector3R L                = LVector / distance;
        const real     LdotN            = L.absDot(cameraNs);
        //const real     LReverseDotEmitN = L.reverse().absDot(emitN);

        SurfaceInfo surfaceInfo;
        surfaceInfo.setPosition(emitP);
        // TODO: refactor here
        surfaceInfo.setGeometryNormal(emitN);
        surfaceInfo.setShadingNormal(emitN);

        PathVertex lightVertex(VertexType::LIGHT_END, radiance / (pickLightPdf * pdfW));
        lightVertex.setSurfaceInfo(surfaceInfo);
        //pdfW * LReverseDotEmitN / (distance * distance));
        lightVertex.setLight(sampleLight);
        lightVertex.setPdfAForward(lightVertex.evaluateOriginPdfA(scene, cameraEndpoint));

        const Spectrum& throughputA = lightVertex.throughput();
        const Spectrum& throughputB = cameraEndpoint.throughput();
        const Spectrum  reflectance = cameraEndpoint.evaluate(TransportMode::RADIANCE, _vertices[t - 2], lightVertex);

        const Spectrum contributeRadiance = throughputA * reflectance * LdotN * throughputB;
        if (contributeRadiance.isZero()) {
            continue;
        }

        // do visibility test only when radiance is valid
        const Ray testRay(cameraP, L, constant::RAY_EPSILON, distance - constant::RAY_EPSILON);
        if (scene.isOccluded(testRay)) {
            continue;
        }

        const real misWeight = BdptMis::weight(
            scene, SubPath::oneVertexPath(lightVertex), *this, 1, t);

        totalRadiance += contributeRadiance * misWeight;
    }

    *out_radiance = totalRadiance;
}

std::size_t SubPath::length() const {
    return _vertices.size();
}

} // namespace cadise