#include "core/integral-tool/hemisphere.h"

#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/vector.h"

#include <cmath>

namespace cadise {

namespace hemisphere {

void uniformSampling(const Vector2R& randomNumber, Vector3R* const out_direction) {
    CADISE_ASSERT(out_direction);

    const real theta = std::acos(randomNumber.x());
    const real phi   = constant::TWO_PI * randomNumber.y();

    const real sinTheta = std::sqrt(1.0_r - randomNumber.x() * randomNumber.x());

    *out_direction =  Vector3R(std::cos(phi) * sinTheta,
                               std::sin(phi) * sinTheta,
                               randomNumber.x());
}

void cosineWeightedSampling(const Vector2R& randomNumber, Vector3R* const out_direction) {
    CADISE_ASSERT(out_direction);

    const real theta = std::acos(1.0_r - 2.0_r * randomNumber.x()) * 0.5_r;
    const real phi   = constant::TWO_PI * randomNumber.y();

    const real cosTheta = std::cos(theta);
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    *out_direction =  Vector3R(std::cos(phi) * sinTheta,
                               std::sin(phi) * sinTheta,
                               cosTheta);
}

} // namespace hemisphere

} // namespace cadise