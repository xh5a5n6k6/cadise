#include "core/camera/perspectivePinholeCamera.h"

#include "core/integral-tool/sample/cameraSample.h"
#include "core/ray.h"
#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/transform.h"

#include <cmath>
#include <limits>

namespace cadise {

PerspectivePinholeCamera::PerspectivePinholeCamera(
    const Vector3R& position,
    const Vector3R& direction, 
    const Vector3R& up, 
    const real      fov,
    const real      sensorWidthMM) :
    
    Camera(position),
    _cameraToWorld(nullptr),
    _filmNdcToCamera(nullptr),
    _fov(fov),
    _sensorWidthMM(sensorWidthMM) {

    _cameraToWorld = std::make_shared<Transform>(Matrix4::lookAt(position, direction, up));

    this->updateTransform();
}

void PerspectivePinholeCamera::updateTransform() {
    const auto [sensorWidth, sensorHeight] = _getSensorSize();

    // update sensorOffset
    _sensorOffset = (sensorWidth / 2.0_r) / std::tan(math::degreeToRadian(_fov / 2.0_r));

    // matrix multiplication is right-hand-side, so we
    // need to initialize matrix first.
    //
    // translate needs to be multiplied last, it means
    // we need to multiply it first (it will be the leftmost part).
    Matrix4 filmNdcToCameraMatrix = Matrix4::identity();
    filmNdcToCameraMatrix *= Matrix4::translate(-(sensorWidth / 2.0_r), sensorHeight / 2.0_r, -_sensorOffset);
    filmNdcToCameraMatrix *= Matrix4::scale(sensorWidth, -sensorHeight, 1.0_r);

    _filmNdcToCamera = std::make_shared<Transform>(filmNdcToCameraMatrix);
}

void PerspectivePinholeCamera::spawnPrimaryRay(
    const Vector2R& filmNdcPosition,
    Ray* const      out_primaryRay) const {

    CADISE_ASSERT(out_primaryRay);

    Vector3R sampleCameraPosition;
    _filmNdcToCamera->transformPoint({filmNdcPosition.x(), filmNdcPosition.y(), 0.0_r}, &sampleCameraPosition);

    // calculate parameter in camera space
    const Vector3R origin = _position;
    Vector3R direction;
    _cameraToWorld->transformVector(sampleCameraPosition, &direction);

    CADISE_ASSERT(!direction.isZero());

    // generate ray in world space
    out_primaryRay->reset();
    out_primaryRay->setOrigin(origin);
    out_primaryRay->setDirection(direction);
}

void PerspectivePinholeCamera::evaluateCameraSample(
    CameraSample* const out_sample, 
    Ray* const          out_toCameraRay) const {

    CADISE_ASSERT(out_sample);
    CADISE_ASSERT(out_toCameraRay);

    Vector3R cameraRayN;
    _cameraToWorld->transformVector({0.0_r, 0.0_r, -1.0_r}, &cameraRayN);

    const Vector3R& targetPosition  = out_sample->targetPosition();
    const Vector3R  cameraRayVector = targetPosition - _position;

    CADISE_ASSERT(!cameraRayVector.isZero());

    const real     sensorArea         = _getSensorArea();
    const real     distance           = cameraRayVector.length();
    const Vector3R cameraRayDirection = cameraRayVector / distance;
    const real     cosTheta           = cameraRayDirection.dot(cameraRayN);
    if (cosTheta <= 0.0_r) {
        return;
    }

    Vector3R targetCameraPosition;
    _cameraToWorld->inverseMatrix().transformPoint(targetPosition, &targetCameraPosition);

    CADISE_ASSERT(!targetCameraPosition.isZero());

    // transform to focus (film) plane in camera space
    const real     cameraToImagePointDistance  = _sensorOffset / cosTheta;
    const real     cameraToImagePointDistance2 = cameraToImagePointDistance * cameraToImagePointDistance;
    const Vector3R targetFocusPosition         = (targetCameraPosition / distance) * cameraToImagePointDistance;

    Vector3R filmNdcPosition;
    _filmNdcToCamera->inverseMatrix().transformPoint(targetFocusPosition, &filmNdcPosition);

    // check NDC boundary (0 ~ 1)
    if (filmNdcPosition.x() <  0.0_r || 
        filmNdcPosition.x() >= 1.0_r ||
        filmNdcPosition.y() <  0.0_r || 
        filmNdcPosition.y() >= 1.0_r) {

        return;
    }

    const real pdfA = 1.0_r;

    out_sample->setCameraPosition(_position);
    out_sample->setCameraNormal(cameraRayN);
    out_sample->setFilmNdcPosition(Vector2R(filmNdcPosition.x(), filmNdcPosition.y()));
    out_sample->setImportance(Spectrum(cameraToImagePointDistance2 / (sensorArea * cosTheta * cosTheta)));
    out_sample->setPdfW(pdfA * distance * distance / cosTheta);

    // shooting from targetPosition
    out_toCameraRay->reset();
    out_toCameraRay->setOrigin(out_sample->targetPosition());
    out_toCameraRay->setDirection(cameraRayDirection.reverse());
    out_toCameraRay->setMaxT(distance - constant::RAY_EPSILON);
}

void PerspectivePinholeCamera::evaluateCameraPdf(
    const Ray&  cameraRay,
    real* const out_pdfA,
    real* const out_pdfW) const {

    CADISE_ASSERT(out_pdfA);
    CADISE_ASSERT(out_pdfW);

    Vector3R cameraRayN;
    _cameraToWorld->transformVector({0.0_r, 0.0_r, -1.0_r}, &cameraRayN);

    const real sensorArea = _getSensorArea();
    const real cosTheta   = cameraRay.direction().dot(cameraRayN);
    if (cosTheta <= 0.0_r) {

        return;
    }

    const real     cameraToImagePointDistance  = _sensorOffset / cosTheta;
    const real     cameraToImagePointDistance2 = cameraToImagePointDistance * cameraToImagePointDistance;
    const Vector3R rayWorldFocusPosition       = cameraRay.at(cameraToImagePointDistance);

    Vector3R rayCameraFocusPosition;
    _cameraToWorld->inverseMatrix().transformPoint(rayWorldFocusPosition, &rayCameraFocusPosition);

    Vector3R filmNdcPosition;
    _filmNdcToCamera->inverseMatrix().transformPoint(rayCameraFocusPosition, &filmNdcPosition);

    // check NDC boundary (0 ~ 1)
    if (filmNdcPosition.x() <  0.0_r ||
        filmNdcPosition.x() >= 1.0_r ||
        filmNdcPosition.y() <  0.0_r ||
        filmNdcPosition.y() >= 1.0_r) {

        return;
    }

    *out_pdfA = 1.0_r;
    *out_pdfW = cameraToImagePointDistance2 / (sensorArea * cosTheta);
}

std::pair<real, real> PerspectivePinholeCamera::_getSensorSize() const {
    return {
        _sensorWidthMM,               // sensorWidth
        _sensorWidthMM / _aspectRatio // sensorHeight
    };
}

real PerspectivePinholeCamera::_getSensorArea() const {
    const auto [sensorWidth, sensorHeight] = _getSensorSize();

    return sensorWidth * sensorHeight;
}

} // namespace cadise