#include "core/integral-tool/hemisphere.h"

#include "math/constant.h"
#include "math/vector.h"

#include <cmath>

namespace cadise {

namespace hemisphere {

Vector3R uniformSampling(const Vector2R& randomNumber) {
    const real theta = std::acos(randomNumber.x());
    const real phi   = constant::TWO_PI * randomNumber.y();

    const real sinTheta = std::sqrt(1.0_r - randomNumber.x() * randomNumber.x());

    return Vector3R(std::cos(phi) * sinTheta,
                    std::sin(phi) * sinTheta,
                    randomNumber.x());
}

Vector3R cosineWeightedSampling(const Vector2R& randomNumber) {
    const real theta = std::acos(1.0_r - 2 * randomNumber.x()) / 2.0_r;
    const real phi   = constant::TWO_PI * randomNumber.y();

    const real cosTheta = std::cos(theta);
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    return Vector3R(std::cos(phi) * sinTheta,
                    std::sin(phi) * sinTheta,
                    cosTheta);
}

} // namespace hemisphere

} // namespace cadise