#include "core/renderer/bdpt-renderer/pathVertex.h"

#include "core/camera/camera.h"
#include "core/light/light.h"
#include "core/ray.h"
#include "core/scene.h"
#include "core/surface/bsdf/bsdf.h"
#include "core/surface/transportInfo.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"

#include <cmath>

namespace cadise {

PathVertex::PathVertex(const VertexType& type) :
    PathVertex(type, Spectrum(0.0_r)) {
}

PathVertex::PathVertex(
    const VertexType& type,
    const Spectrum&   throughput) :

    _type(type),
    _throughput(throughput),
    _surfaceInfo(),
    _pdfAForward(0.0_r),
    _pdfAReverse(0.0_r),
    _camera(nullptr),
    _light(nullptr),
    _bsdf(nullptr) {
}

bool PathVertex::isConnectible() const {
    switch (_type) {
        case VertexType::CAMERA_END:
            CADISE_ASSERT(_camera);

            return true;

        case VertexType::LIGHT_END:
            CADISE_ASSERT(_light);

            return !_light->isDeltaLight();

        case VertexType::SURFACE:
            CADISE_ASSERT(_bsdf);

            return _bsdf->lobes().hasAtLeastOne({
                ELobe::DIFFUSE_REFLECTION,
                ELobe::DIFFUSE_TRANSMISSION,
                ELobe::GLOSSY_REFLECTION,
                ELobe::GLOSSY_TRANSMISSION });

        default:
            // unreachable place
            return false;
    }

    // unreachable place
    return false;
}

Spectrum PathVertex::evaluate(
    const TransportMode& mode,
    const PathVertex&    previous, 
    const PathVertex&    next) const {

    CADISE_ASSERT(_bsdf);

    if (!this->isConnectible()) {
        return Spectrum(0.0_r);
    }

    const Vector3R toPrevious = previous.surfaceInfo().position() - _surfaceInfo.position();
    const Vector3R toNext     = next.surfaceInfo().position() - _surfaceInfo.position();

    CADISE_ASSERT(!toPrevious.isZero());
    CADISE_ASSERT(!toNext.isZero());

    SurfaceIntersection intersection;
    intersection.setSurfaceInfo(_surfaceInfo);
    intersection.setWi(toPrevious.normalize());
    intersection.setWo(toNext.normalize());

    Spectrum reflectance = _bsdf->evaluate(TransportInfo(mode), intersection);
    if (mode == TransportMode::IMPORTANCE) {
        reflectance *= 1.0_r;
    }

    return reflectance;
}

real PathVertex::evaluateOriginPdfA(
    const Scene&      scene,
    const PathVertex& next) const {
    
    const Vector3R& nowP  = _surfaceInfo.position();
    const Vector3R& nextP = next.surfaceInfo().position();
    if (nowP.isEqualTo(nextP)) {
        return 0.0_r;
    }

    CADISE_ASSERT(!(_camera && _light));

    if (_camera) {
        real pdfA;
        real pdfW;
        _camera->evaluateCameraPdf(Ray(nowP, nextP - nowP), &pdfA, &pdfW);

        return pdfA;
    }
    else if (_light) {
        const real pickLightPdf = scene.evaluatePickLightPdf(_light);

        real pdfA;
        real pdfW;
        _light->evaluateEmitPdf(Ray(nowP, nextP - nowP), 
                                _surfaceInfo.shadingNormal(),
                                &pdfA, 
                                &pdfW);

        return pickLightPdf * pdfA;
    }

    // unreachable place
    return 0.0_r;
}

real PathVertex::evaluateDirectPdfA(
    const Scene&      scene,
    const PathVertex& next) const {

    const Vector3R& nowP  = _surfaceInfo.position();
    const Vector3R& nextP = next.surfaceInfo().position();
    if (nowP.isEqualTo(nextP)) {
        return 0.0_r;
    }

    CADISE_ASSERT(!(_camera && _light));

    const Vector3R& nextNs    = next.surfaceInfo().shadingNormal();
    const Vector3R  nowToNext = nextP - nowP;
    
    const real distance2     = nowToNext.lengthSquared();
    const real nextToNowDotN = (nowToNext / std::sqrt(distance2)).reverse().absDot(nextNs);

    if (_camera) {
        real pdfA;
        real pdfW;
        _camera->evaluateCameraPdf(Ray(nowP, nextP - nowP), &pdfA, &pdfW);

        return pdfW * nextToNowDotN / distance2;
    }
    else if (_light) {
        real pdfA;
        real pdfW;
        _light->evaluateEmitPdf(Ray(nowP, nextP - nowP), 
                                _surfaceInfo.shadingNormal(),
                                &pdfA,
                                &pdfW);

        return pdfW * nextToNowDotN / distance2;
    }

    // unreachable place
    return 0.0_r;
}

real PathVertex::evaluateConnectPdfA(
    const TransportMode& mode,
    const PathVertex&    previous,
    const PathVertex&    next) const {

    const Vector3R& nowP      = _surfaceInfo.position();
    const Vector3R& previousP = previous.surfaceInfo().position();
    const Vector3R& nextP     = next.surfaceInfo().position();
    if (nowP.isEqualTo(nextP) || nowP.isEqualTo(previousP)) {
        return 0.0_r;
    }

    CADISE_ASSERT(_bsdf);

    const Vector3R& nextNs        = next.surfaceInfo().shadingNormal();
    const Vector3R  nowToPrevious = previousP - nowP;
    const Vector3R  nowToNext     = nextP - nowP;

    const real distance2     = nowToNext.lengthSquared();
    const real nextToNowDotN = (nowToNext / std::sqrt(distance2)).reverse().absDot(nextNs);

    // TODO: refactor here
    SurfaceIntersection intersection;
    intersection.setSurfaceInfo(_surfaceInfo);
    intersection.setWi(nowToPrevious.normalize());
    intersection.setWo(nowToNext.normalize());

    const real pdfW = _bsdf->evaluatePdfW(TransportInfo(mode), intersection);

    real pdfA = pdfW / distance2;
    if (next.camera() == nullptr) {
        pdfA *= nextToNowDotN;
    }

    return pdfA;
}

const VertexType& PathVertex::type() const {
    return _type;
}

const Spectrum& PathVertex::throughput() const {
    return _throughput;
}

const SurfaceInfo& PathVertex::surfaceInfo() const {
    return _surfaceInfo;
}

real PathVertex::pdfAForward() const {
    return _pdfAForward;
}

real PathVertex::pdfAReverse() const {
    return _pdfAReverse;
}

const Camera* PathVertex::camera() const {
    return _camera;
}

const Light* PathVertex::light() const {
    return _light;
}

const Bsdf* PathVertex::bsdf() const {
    return _bsdf;
}

void PathVertex::setSurfaceInfo(const SurfaceInfo& surfaceInfo) {
    _surfaceInfo = surfaceInfo;
}

void PathVertex::setPdfAForward(const real pdfAForward) {
    _pdfAForward = pdfAForward;
}

void PathVertex::setPdfAReverse(const real pdfAReverse) {
    _pdfAReverse = pdfAReverse;
}

void PathVertex::setCamera(const Camera* const camera) {
    CADISE_ASSERT(camera);

    _camera = camera;
}

void PathVertex::setLight(const Light* const light) {
    CADISE_ASSERT(light);

    _light = light;
}

void PathVertex::setBsdf(const Bsdf* const bsdf) {
    CADISE_ASSERT(bsdf);

    _bsdf = bsdf;
}

} // namespace cadise