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
    const float64   fov,
    const float64   sensorWidthMM) :
    
    Camera(position),
    _cameraToWorld(nullptr),
    _filmToCamera(nullptr),
    _fov(fov),
    _sensorWidthMM(sensorWidthMM) {

    _cameraToWorld = std::make_unique<Transform>(Matrix4R::lookAt(position, direction, up));

    this->updateTransform();
}

void PerspectivePinholeCamera::updateTransform() {
    const auto [sensorWidth, sensorHeight] = _getSensorSizeXy();
    const auto realResolution              = _resolution.asType<real>();

    // update sensorOffset
    _sensorOffset = (sensorWidth * 0.5_r) / std::tan(math::degree_to_radian(_fov * 0.5_r));

    // matrix multiplication is right-hand-side, so we
    // need to initialize matrix first.
    //
    // translate needs to be multiplied last, it means
    // we need to multiply it first (it will be the leftmost part).
    Matrix4R filmToCameraMatrix = Matrix4R::identity();
    filmToCameraMatrix *= Matrix4R::translate(-(sensorWidth * 0.5_r), sensorHeight * 0.5_r, -_sensorOffset);
    filmToCameraMatrix *= Matrix4R::scale(
        sensorWidth / realResolution.x(), -sensorHeight / realResolution.y(), 1.0_r);

    _filmToCamera = std::make_unique<Transform>(filmToCameraMatrix);
}

void PerspectivePinholeCamera::spawnPrimaryRay(
    const Vector2D& filmPosition,
    Ray* const      out_primaryRay) const {

    CADISE_ASSERT(out_primaryRay);

    Vector3R sampleCameraPosition;
    _filmToCamera->transformPoint(
        Vector3D(filmPosition.x(), filmPosition.y(), 0.0_r).asType<real>(), 
        &sampleCameraPosition);

    // calculate parameter in camera space
    Vector3R direction;
    _cameraToWorld->transformVector(sampleCameraPosition, &direction);

    CADISE_ASSERT(!direction.isZero());

    // generate ray in world space
    out_primaryRay->reset();
    out_primaryRay->setOrigin(_position);
    out_primaryRay->setDirection(direction);
}

void PerspectivePinholeCamera::evaluateCameraSample(
    CameraSample* const out_sample, 
    Ray* const          out_toCameraRay) const {

    CADISE_ASSERT(out_sample);
    CADISE_ASSERT(out_toCameraRay);

    Vector3R cameraRayN;
    _cameraToWorld->transformVector(Vector3R(0.0_r, 0.0_r, -1.0_r), &cameraRayN);

    const Vector3R targetPosition  = out_sample->targetPosition();
    const Vector3R cameraRayVector = targetPosition - _position;

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

    // transform from camera space to film (raster) space
    Vector3R filmPosition;
    _filmToCamera->inverseMatrix().transformPoint(targetFocusPosition, &filmPosition);

    // check film boundary (0 ~ resolution)
    if (filmPosition.x() <  0.0_r || 
        filmPosition.y() <  0.0_r ||
        filmPosition.x() >= static_cast<real>(_resolution.x()) || 
        filmPosition.y() >= static_cast<real>(_resolution.y())) {

        // temporary hack for BDPG radiance estimator
        // TODO: remove this situation
        if (out_toCameraRay->origin().isEqualTo(targetPosition)) {
            fprintf(stdout, "filmPosition error\n");
            fprintf(stdout, "cos: %f, x: %lf, y: %f\n", cosTheta, double(filmPosition.x()), filmPosition.y());

            out_sample->setCameraPosition(_position);
            out_sample->setCameraNormal(cameraRayN);
            out_sample->setFilmPosition(Vector2D(filmPosition.x(), filmPosition.y()));
            out_sample->setImportance(Spectrum(cameraToImagePointDistance2 / (sensorArea * cosTheta * cosTheta)));
            out_sample->setPdfW(1.0_r * distance * distance / cosTheta);
        }

        return;
    }

    const real pdfA = 1.0_r;

    out_sample->setCameraPosition(_position);
    out_sample->setCameraNormal(cameraRayN);
    out_sample->setFilmPosition(Vector2D(filmPosition.x(), filmPosition.y()));
    out_sample->setImportance(Spectrum(cameraToImagePointDistance2 / (sensorArea * cosTheta * cosTheta)));
    out_sample->setPdfW(pdfA * distance * distance / cosTheta);

    // shooting from targetPosition
    out_toCameraRay->reset();
    out_toCameraRay->setOrigin(targetPosition);
    out_toCameraRay->setDirection(cameraRayDirection.reverse());
    out_toCameraRay->setMaxT(distance - constant::ray_epsilon<real>);
}

void PerspectivePinholeCamera::evaluateCameraPdf(
    const Ray&  cameraRay,
    real* const out_pdfA,
    real* const out_pdfW) const {

    CADISE_ASSERT(out_pdfA);
    CADISE_ASSERT(out_pdfW);

    Vector3R cameraRayN;
    _cameraToWorld->transformVector(Vector3R(0.0_r, 0.0_r, -1.0_r), &cameraRayN);

    const real cosTheta = cameraRay.direction().dot(cameraRayN);
    if (cosTheta <= 0.0_r) {
        return;
    }

    const float64  sensorArea                  = _getSensorArea();
    const float64  cameraToImagePointDistance  = _sensorOffset / cosTheta;
    const float64  cameraToImagePointDistance2 = cameraToImagePointDistance * cameraToImagePointDistance;
    const Vector3R rayWorldFocusPosition       = cameraRay.at(cameraToImagePointDistance);

    Vector3R rayCameraFocusPosition;
    _cameraToWorld->inverseMatrix().transformPoint(rayWorldFocusPosition, &rayCameraFocusPosition);

    Vector3R filmPosition;
    _filmToCamera->inverseMatrix().transformPoint(rayCameraFocusPosition, &filmPosition);

    // check film boundary (0 ~ resolution)
    if (filmPosition.x() <  0.0_r ||
        filmPosition.y() <  0.0_r ||
        filmPosition.x() >= static_cast<real>(_resolution.x()) ||
        filmPosition.y() >= static_cast<real>(_resolution.y())) {

        return;
    }

    *out_pdfA = 1.0_r;
    *out_pdfW = cameraToImagePointDistance2 / (sensorArea * cosTheta);
}

std::pair<float64, float64> PerspectivePinholeCamera::_getSensorSizeXy() const {
    const float64 aspectRatio = _getAspectRatio();

    return {
        _sensorWidthMM,              // sensorWidth
        _sensorWidthMM / aspectRatio // sensorHeight
    };
}

float64 PerspectivePinholeCamera::_getSensorArea() const {
    const auto [sensorWidth, sensorHeight] = _getSensorSizeXy();

    return sensorWidth * sensorHeight;
}

} // namespace cadise