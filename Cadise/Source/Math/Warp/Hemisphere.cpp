#include "Math/Warp/Hemisphere.h"

#include "Foundation/Assertion.h"
#include "Math/Constant.h"
#include "Math/TVector3.h"

#include <cmath>

namespace cadise
{

Vector3R Hemisphere::uniformSampling(const std::array<real, 2>& sample)
{
    real dummyPdfW;
    return Hemisphere::uniformSampling(sample, &dummyPdfW);
}

Vector3R Hemisphere::uniformSampling(
    const std::array<real, 2>& sample,
    real* const                out_pdfW)
{
    CS_ASSERT(out_pdfW);

    const real phi      = constant::two_pi<real> * sample[0];
    const real cosTheta = sample[1];
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    *out_pdfW = constant::rcp_two_pi<real>;

    return
    {
        std::sin(phi) * sinTheta,
        cosTheta,
        std::cos(phi) * sinTheta
    };
}

Vector3R Hemisphere::cosineWeightedSampling(const std::array<real, 2>& sample)
{
    real dummyPdfW;
    return Hemisphere::cosineWeightedSampling(sample, &dummyPdfW);
}

Vector3R Hemisphere::cosineWeightedSampling(
    const std::array<real, 2>& sample,
    real* const                out_pdfW)
{
    CS_ASSERT(out_pdfW);

    const real phi      = constant::two_pi<real> * sample[0];
    const real theta    = std::acos(1.0_r - 2.0_r * sample[1]) * 0.5_r;
    const real cosTheta = std::cos(theta);
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    *out_pdfW = cosTheta * constant::rcp_pi<real>;

    return
    {
        std::sin(phi) * sinTheta,
        cosTheta,
        std::cos(phi) * sinTheta
    };
}

Vector3R Hemisphere::cosineExpWeightedSampling(
    const std::array<real, 2>& sample,
    const real                 exponent)
{
    real dummyPdfW;
    return Hemisphere::cosineExpWeightedSampling(sample, exponent, &dummyPdfW);
}

Vector3R Hemisphere::cosineExpWeightedSampling(
    const std::array<real, 2>& sample,
    const real                 exponent,
    real* const                out_pdfW)
{
    CS_ASSERT(out_pdfW);

    const real phi      = constant::two_pi<real> * sample[0];
    const real cosTheta = std::pow(sample[1], 1.0_r / (exponent + 1.0_r));
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    *out_pdfW = (exponent + 1.0_r) * constant::rcp_two_pi<real> * std::pow(cosTheta, exponent);

    return
    {
        std::sin(phi) * sinTheta,
        cosTheta,
        std::cos(phi) * sinTheta
    };
}

} // namespace cadise