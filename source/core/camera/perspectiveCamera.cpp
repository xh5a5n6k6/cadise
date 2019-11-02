#include "core/camera/perspectiveCamera.h"

#include "core/ray.h"

#include "math/constant.h"
#include "math/transform.h"

#include <cmath>
#include <limits>

namespace cadise {

PerspectiveCamera::PerspectiveCamera(const Vector3R& position, const Vector3R& direction, 
                                     const Vector3R& up, const real fov) :
    Camera(position),
    _cameraToWorld(nullptr),
    _filmToCamera(nullptr),
    _fov(fov),
    _sensorWidthMM(36.0_r) {

    _cameraToWorld = std::make_shared<Transform>(Matrix4::lookAt(position, direction, up));
}

void PerspectiveCamera::updateTransform() {
    const real sensorWidth  = _sensorWidthMM;
    const real sensorHeight = sensorWidth / _aspectRatio;
    const real sensorOffset = (sensorWidth / 2.0_r) / std::tan(math::degreeToRadian(_fov / 2.0_r));

    // matrix multiplication is right-hand-side, so we
    // need to initialize matrix first.
    //
    // translate needs to be multiplication last, it means
    // we need to multiply it first (it will be the leftmost part).
    Matrix4 filmToCameraMatrix = Matrix4::identity();
    filmToCameraMatrix *= Matrix4::translate(-(sensorWidth / 2.0_r), sensorHeight / 2.0_r, -sensorOffset);
    filmToCameraMatrix *= Matrix4::scale(sensorWidth, -sensorHeight, 1.0_r);

    _filmToCamera = std::make_shared<Transform>(filmToCameraMatrix);
}

Ray PerspectiveCamera::spawnPrimaryRay(const Vector2R& filmNdcPosition) const {
    const Vector3R sampleCameraPosition = 
        _filmToCamera->transformPoint(Vector3R(filmNdcPosition.x(), filmNdcPosition.y(), 0.0_r));

    // calculate parameter in camera space
    const Vector3R origin = _position;
    const Vector3R direction = _cameraToWorld->transformVector(sampleCameraPosition).normalize();

    // generate ray in world space
    return Ray(origin, 
               direction, 
               constant::RAY_EPSILON, 
               std::numeric_limits<real>::max());
}

} // namespace cadise