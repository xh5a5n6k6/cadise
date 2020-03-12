#include "core/renderer/bdpt-renderer/subPathBuilder.h"

#include "core/camera/camera.h"
#include "core/integral-tool/russianRoulette.h"
#include "core/integral-tool/sample/directLightSample.h"
#include "core/integral-tool/sample/emitLightSample.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/category/areaLight.h"
#include "core/ray.h"
#include "core/renderer/bdpt-renderer/bdptMis.h"
#include "core/renderer/bdpt-renderer/subPath.h"
#include "core/scene.h"
#include "core/surface/bsdf/bsdf.h"
#include "core/surface/transportInfo.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"

namespace cadise {

SubPathBuilder::SubPathBuilder(const std::size_t maxPathLength) :
    _maxPathLength(maxPathLength),
    _camera(nullptr) {
}

void SubPathBuilder::setCamera(const Camera* const camera) {
    CADISE_ASSERT(camera);

    _camera = camera;
}

void SubPathBuilder::buildLightPath(
    const Scene&   scene,
    SubPath* const out_lightPath) const {

    CADISE_ASSERT(out_lightPath);

    real pickLightPdf;
    const Light* sampleLight = scene.sampleOneLight(&pickLightPdf);

    CADISE_ASSERT(sampleLight);

    EmitLightSample emitLightSample;
    sampleLight->evaluateEmitSample(&emitLightSample);
    if (!emitLightSample.isValid()) {
        return;
    }

    const Spectrum& emittance     = emitLightSample.radiance();
    const Vector3R& emitPosition  = emitLightSample.emitPosition();
    const Vector3R& emitDirection = emitLightSample.emitDirection();
    const Vector3R& emitN         = emitLightSample.emitNormal();
    const real      emitPdfA      = emitLightSample.pdfA();
    const real      emitPdfW      = emitLightSample.pdfW();
    const real      pdfAForward   = pickLightPdf * emitPdfA;

    SurfaceInfo surfaceInfo;
    surfaceInfo.setPosition(emitPosition);
    surfaceInfo.setGeometryNormal(emitN);
    surfaceInfo.setShadingNormal(emitN);

    PathVertex lightVertex(VertexType::LIGHT_END, emittance);
    lightVertex.setSurfaceInfo(surfaceInfo);
    lightVertex.setPdfAForward(pdfAForward);
    lightVertex.setLight(sampleLight);

    out_lightPath->addVertex(lightVertex);

    const real     numerator   = emitDirection.absDot(emitN);
    const real     denominator = pickLightPdf * emitPdfA * emitPdfW;
    const Spectrum throughput  = emittance * (numerator / denominator);

    // it will not used in light sub-path construction
    Spectrum localRadiance;
    _buildSubPathCompletely(TransportMode::IMPORTANCE,
                            scene,
                            Ray(emitPosition, emitDirection),
                            throughput,
                            emitPdfW,
                            out_lightPath,
                            &localRadiance);
}

void SubPathBuilder::buildCameraPath(
    const Scene&    scene,
    const Vector2R& filmNdcPosition,
    SubPath* const  out_cameraPath,
    Spectrum* const out_zeroBounceRadiance) const {

    CADISE_ASSERT(out_cameraPath);
    CADISE_ASSERT(out_zeroBounceRadiance);
    CADISE_ASSERT(_camera);

    Ray primaryRay;
    _camera->spawnPrimaryRay(filmNdcPosition, &primaryRay);

    real pdfA;
    real pdfW;
    _camera->evaluateCameraPdf(primaryRay, &pdfA, &pdfW);

    SurfaceInfo surfaceInfo;
    surfaceInfo.setPosition(primaryRay.origin());
    // TODO: refactor here
    const Vector3R cameraN(0.0_r, 0.0_r, -1.0_r);
    surfaceInfo.setGeometryNormal(cameraN);
    surfaceInfo.setShadingNormal(cameraN);

    PathVertex cameraVertex(VertexType::CAMERA_END, Spectrum(1.0_r));
    cameraVertex.setSurfaceInfo(surfaceInfo);
    cameraVertex.setPdfAForward(pdfA);
    cameraVertex.setCamera(_camera);

    out_cameraPath->addVertex(cameraVertex);

    _buildSubPathCompletely(TransportMode::RADIANCE,
                            scene,
                            primaryRay,
                            Spectrum(1.0_r),
                            pdfW,
                            out_cameraPath,
                            out_zeroBounceRadiance);
}

void SubPathBuilder::_buildSubPathCompletely(
    const TransportMode& mode,
    const Scene&         scene,
    const Ray&           firstRay,
    const Spectrum&      secondVertexThroughput,
    const real           secondVertexForwardPdfW,
    SubPath* const       out_subPath,
    Spectrum* const      out_zeroBounceRadiance) const {

    CADISE_ASSERT(out_subPath);

    std::size_t currentLength = out_subPath->length();
    if (currentLength == _maxPathLength) {
        return;
    }

    Ray      traceRay(firstRay);
    Spectrum throughput  = secondVertexThroughput;
    real     pdfWForward = secondVertexForwardPdfW;
    real     pdfWReverse = 0.0_r;

    while (true) {
        SurfaceIntersection intersection;
        if (!scene.isIntersecting(traceRay, intersection)) {
            break;
        }

        PathVertex& previousVertex = (*out_subPath)[currentLength - 1];
        PathVertex  newVertex(VertexType::SURFACE, throughput);

        const Primitive* primitive = intersection.primitiveInfo().primitive();
        const Bsdf*      bsdf      = primitive->bsdf();

        const Vector3R& newP       = intersection.surfaceInfo().position();
        const Vector3R& newNs      = intersection.surfaceInfo().shadingNormal();
        const Vector3R& previousP  = previousVertex.surfaceInfo().position();
        const Vector3R& previousNs = previousVertex.surfaceInfo().shadingNormal();

        const real distance          = traceRay.maxT();
        const real distance2         = distance * distance;
        const real previousToNewDotN = traceRay.direction().absDot(previousNs);
        const real newToPreviousDotN = traceRay.direction().reverse().absDot(newNs);

        newVertex.setSurfaceInfo(intersection.surfaceInfo());
        newVertex.setPdfAForward(pdfWForward * newToPreviousDotN / distance2);
        newVertex.setBsdf(bsdf);

        out_subPath->addVertex(newVertex);
        ++currentLength;

        // add s=0 situation radiance when hitting area light
        // (while building camera sub-path)
        const AreaLight* areaLight = primitive->areaLight();
        if (areaLight && mode == TransportMode::RADIANCE) {
            (*out_subPath)[currentLength - 1].setLight(areaLight);

            const Spectrum emittance = areaLight->emittance(intersection);
            const real     misWeight = BdptMis::weight(scene, SubPath::emptyPath(), *out_subPath, 0, currentLength);

            *out_zeroBounceRadiance += misWeight * emittance * throughput;
        }

        if (currentLength == _maxPathLength) {
            break;
        }

        const Spectrum  reflectance = bsdf->evaluateSample(TransportInfo(mode), intersection);
        const Vector3R& L = intersection.wo();

        const real LdotN = L.absDot(newNs);

        // for non-symmetric scattering correction
        throughput *= reflectance * LdotN / intersection.pdf();
        if (mode == TransportMode::IMPORTANCE) {
            throughput *= 1.0_r;
        }

        pdfWForward = intersection.pdf();
        pdfWReverse = bsdf->evaluatePdfW(TransportInfo(mode), intersection.reverse());

        // for specular surface, it is impossible
        // to connect from both sides
        if (!newVertex.isConnectible()) {
            pdfWForward = 0.0_r;
            pdfWReverse = 0.0_r;
        }

        previousVertex.setPdfAReverse(pdfWReverse * previousToNewDotN / distance2);

        if (reflectance.isZero() || intersection.pdf() == 0.0_r) {
            break;
        }

        // use russian roulette to decide if the ray needs to be kept tracking
        if (currentLength > 4) {
            Spectrum newThroughput;
            if (RussianRoulette::isSurvivedOnNextRound(throughput, &newThroughput)) {
                throughput = newThroughput;
            }
            else {
                break;
            }
        }

        if (throughput.isZero()) {
            break;
        }

        traceRay.reset();
        traceRay.setOrigin(newP);
        traceRay.setDirection(L);
    }
}

} // namespace cadise