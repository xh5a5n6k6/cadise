#include "Math/Warp/Disk.h"

#include "Foundation/Assertion.h"
#include "Math/Constant.h"
#include "Math/TVector2.h"

#include <cmath>

namespace cadise::internal
{

// Return (radius, theta)
std::pair<real, real> mapToRadiusTheta(const Vector2R& sample)
{
    if (std::abs(sample[0]) > std::abs(sample[1]))
    {
        return
        {
            sample[0],
            constant::pi_over_four<real> * (sample[1] / sample[0])
        };
    }
    else
    {
        return
        {
            sample[1],
            constant::pi_over_two<real> - constant::pi_over_four<real> * (sample[0] / sample[1])
        };
    }
}

} // namespace cadise::internal

namespace cadise
{

void Disk::uniformSampling(
    const std::array<real, 2>& sample,
    Vector2R* const            out_surfacePoint)
{
    CS_ASSERT(out_surfacePoint);

    const real sampledRadius = std::sqrt(sample[0]);
    const real sampledTheta  = constant::two_pi<real> * sample[1];
    const real cosTheta      = std::cos(sampledTheta);
    const real sinTheta      = std::sqrt(1.0_r - cosTheta * cosTheta);

    out_surfacePoint->set(sampledRadius * cosTheta, sampledRadius * sinTheta);
}

void Disk::concentricSampling(
    const std::array<real, 2>& sample,
    Vector2R* const            out_surfacePoint)
{
    CS_ASSERT(out_surfacePoint);

    // Map sample from [0, 1] to [-1, 1]
    const Vector2R mappedSample = Vector2R(sample).mul(2.0_r).sub(1.0_r);
    if (mappedSample.isZero())
    {
        out_surfacePoint->set(0.0_r);
        return;
    }

    const auto [sampledRadius, sampledTheta] = internal::mapToRadiusTheta(mappedSample);
    const real cosTheta = std::cos(sampledTheta);
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    out_surfacePoint->set(sampledRadius * cosTheta, sampledRadius * sinTheta);
}

} // namespace cadise