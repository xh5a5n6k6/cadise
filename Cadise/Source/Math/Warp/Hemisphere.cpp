#include "math/warp/hemisphere.h"

#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/tVector3.h"

#include <cmath>

namespace cadise 
{

void Hemisphere::uniformSampling(
    const std::array<real, 2>& sample,
    Vector3R* const            out_direction,
    real* const                out_pdfW) 
{
    CS_ASSERT(out_direction);
    CS_ASSERT(out_pdfW);

    const real phi      = constant::two_pi<real> * sample[0];
    const real cosTheta = sample[1];
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    out_direction->set(
        std::sin(phi) * sinTheta,
        cosTheta,
        std::cos(phi) * sinTheta);

    *out_pdfW = constant::rcp_two_pi<real>;
}

void Hemisphere::cosineWeightedSampling(
    const std::array<real, 2>& sample,
    Vector3R* const            out_direction,
    real* const                out_pdfW) 
{
    CS_ASSERT(out_direction);
    CS_ASSERT(out_pdfW);

    const real phi      = constant::two_pi<real> * sample[0];
    const real theta    = std::acos(1.0_r - 2.0_r * sample[1]) * 0.5_r;
    const real cosTheta = std::cos(theta);
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    out_direction->set(
        std::sin(phi) * sinTheta,
        cosTheta,
        std::cos(phi) * sinTheta);

    *out_pdfW = cosTheta * constant::rcp_pi<real>;
}

void Hemisphere::cosineExpWeightedSampling(
    const std::array<real, 2>& sample,
    const real                 exponent,
    Vector3R* const            out_direction,
    real* const                out_pdfW) 
{
    CS_ASSERT(out_direction);
    CS_ASSERT(out_pdfW);

    const real phi      = constant::two_pi<real> * sample[0];
    const real cosTheta = std::pow(sample[1], 1.0_r / (exponent + 1.0_r));
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    out_direction->set(
        std::sin(phi) * sinTheta,
        cosTheta,
        std::cos(phi) * sinTheta);

    *out_pdfW = (exponent + 1.0_r) * constant::rcp_two_pi<real> * std::pow(cosTheta, exponent);
}

} // namespace cadise