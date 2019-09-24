#include "core/camera/perspectiveCamera.h"

#include "core/ray.h"

#include "math/constant.h"
#include "math/math.h"
#include "math/random.h"

#include <cmath>
#include <limits>

namespace cadise {

PerspectiveCamera::PerspectiveCamera(const Film& film, const real fov, const Vector3R* const lookAt) :
    _film(film),
    _fov(fov),
    _cameraToWorld(Matrix4::lookAt(lookAt[0], lookAt[1], lookAt[2])) {

    const real rx = static_cast<real>(_film.resolution().x());
    const real ry = static_cast<real>(_film.resolution().y());
    const real aspectRatio = ry / rx;

    const real screenWidth  = 2.0_r * std::tan(math::degreeToRadian(_fov / 2.0_r));
    const real screenHeight = screenWidth * aspectRatio;

    _pixelWidth  = screenWidth / rx;
    _pixelHeight = screenHeight / ry;
    _leftUpFilmPosition = Vector2R(-0.5_r * screenWidth, 0.5_r * screenHeight);
}

Ray PerspectiveCamera::spawnPrimaryRay(const Vector2R& pixelPosition) const {
    const real x = _leftUpFilmPosition.x();
    const real y = _leftUpFilmPosition.y();

    const Vector3R samplePoint = Vector3R(x + pixelPosition.x() * _pixelWidth, 
                                          y - pixelPosition.y() * _pixelHeight,
                                          -1.0_r);

    const Vector3R origin    = _cameraToWorld.transformPoint(Vector3R(0.0_r, 0.0_r, 0.0_r));
    const Vector3R direction = _cameraToWorld.transformVector(samplePoint);

    // generate ray in world space
    return Ray(origin, 
               direction, 
               constant::RAY_EPSILON, 
               std::numeric_limits<real>::max());
}

Film PerspectiveCamera::film() const {
    return _film;
}

} // namespace cadise