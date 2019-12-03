#include "core/integral-tool/hemisphere.h"

#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/vector.h"

#include <cmath>

namespace cadise {

void Hemisphere::uniformSampling(const Vector2R& sample, 
                                 Vector3R* const out_direction) {

    CADISE_ASSERT(out_direction);

    const real phi      = constant::TWO_PI * sample.x();
    const real cosTheta = sample.y();
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    *out_direction =  Vector3R(std::cos(phi) * sinTheta,
                               std::sin(phi) * sinTheta,
                               cosTheta);
}

void Hemisphere::cosineWeightedSampling(const Vector2R& sample, 
                                        Vector3R* const out_direction) {

    CADISE_ASSERT(out_direction);

    const real phi   = constant::TWO_PI * sample.x();
    const real theta = std::acos(1.0_r - 2.0_r * sample.y()) * 0.5_r;

    const real cosTheta = std::cos(theta);
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    *out_direction =  Vector3R(std::cos(phi) * sinTheta,
                               std::sin(phi) * sinTheta,
                               cosTheta);
}

void Hemisphere::cosineExpWeightedSampling(const real exponent,
                                           const Vector2R& sample,
                                           Vector3R* const out_direction) {

    CADISE_ASSERT(out_direction);

    const real phi      = constant::TWO_PI * sample.x();
    const real cosTheta = std::pow(sample.y(), 1.0_r / (exponent + 1.0_r));
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    *out_direction = Vector3R(std::cos(phi) * sinTheta,
                              std::sin(phi) * sinTheta,
                              cosTheta);
}

} // namespace cadise