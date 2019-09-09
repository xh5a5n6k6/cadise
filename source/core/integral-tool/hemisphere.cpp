#include "core/integral-tool/hemisphere.h"

#include "math/constant.h"
#include "math/vector.h"

#include <cmath>

namespace cadise {

namespace hemisphere {

Vector3R uniformSampling(const Vector2R& randomNumber) {
    real theta = std::acos(randomNumber.x());
    real phi = constant::TWO_PI * randomNumber.y();

    real sinTheta = std::sqrt(1.0_r - randomNumber.x() * randomNumber.x());

    return Vector3R(std::cos(phi) * sinTheta,
                    std::sin(phi) * sinTheta,
                    randomNumber.x());
}

Vector3R cosineWeightedSampling(const Vector2R& randomNumber) {
    real theta = std::acos(1.0_r - 2 * randomNumber.x()) / 2.0_r;
    real phi = constant::TWO_PI * randomNumber.y();

    real cosTheta = std::cos(theta);
    real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    return Vector3R(std::cos(phi) * sinTheta,
                    std::sin(phi) * sinTheta,
                    cosTheta);
}

} // namespace hemisphere

} // namespace cadise