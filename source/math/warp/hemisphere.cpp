#include "math/warp/hemisphere.h"

#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/vector.h"

#include <cmath>

namespace cadise {

void Hemisphere::uniformSampling(
    const Vector2R& sample, 
    Vector3R* const out_direction,
    real* const     out_pdfW) {

    CADISE_ASSERT(out_direction);
    CADISE_ASSERT(out_pdfW);

    const real phi      = constant::two_pi<real> * sample.x();
    const real cosTheta = sample.y();
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    *out_direction =  Vector3R(std::cos(phi) * sinTheta,
                               std::sin(phi) * sinTheta,
                               cosTheta);

    *out_pdfW = constant::inv_two_pi<real>;
}

void Hemisphere::cosineWeightedSampling(
    const Vector2R& sample, 
    Vector3R* const out_direction,
    real* const     out_pdfW) {

    CADISE_ASSERT(out_direction);
    CADISE_ASSERT(out_pdfW);

    const real phi      = constant::two_pi<real> * sample.x();
    const real theta    = std::acos(1.0_r - 2.0_r * sample.y()) * 0.5_r;
    const real cosTheta = std::cos(theta);
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    *out_direction =  Vector3R(std::cos(phi) * sinTheta,
                               std::sin(phi) * sinTheta,
                               cosTheta);

    *out_pdfW = cosTheta * constant::inv_pi<real>;
}

void Hemisphere::cosineExpWeightedSampling(
    const Vector2R& sample,
    const real      exponent,
    Vector3R* const out_direction,
    real* const     out_pdfW) {

    CADISE_ASSERT(out_direction);
    CADISE_ASSERT(out_pdfW);

    const real phi      = constant::two_pi<real> * sample.x();
    const real cosTheta = std::pow(sample.y(), 1.0_r / (exponent + 1.0_r));
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    *out_direction = Vector3R(std::cos(phi) * sinTheta,
                              std::sin(phi) * sinTheta,
                              cosTheta);

    *out_pdfW = (exponent + 1.0_r) * constant::inv_two_pi<real> * std::pow(cosTheta, exponent);
}

} // namespace cadise