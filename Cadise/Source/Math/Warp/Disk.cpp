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

Vector2R Disk::uniformSampling(const std::array<real, 2>& sample)
{
    const real sampledRadius = std::sqrt(sample[0]);
    const real sampledTheta  = constant::two_pi<real> * sample[1];
    const real cosTheta      = std::cos(sampledTheta);
    const real sinTheta      = std::sqrt(1.0_r - cosTheta * cosTheta);

    return
    {
        sampledRadius * cosTheta,
        sampledRadius * sinTheta
    };
}

Vector2R Disk::concentricSampling(const std::array<real, 2>& sample)
{
    // Map sample from [0, 1] to [-1, 1]
    const Vector2R mappedSample = Vector2R(sample).mul(2.0_r).sub(1.0_r);
    if (mappedSample.isZero())
    {
        return Vector2R(0.0_r);
    }

    const auto [sampledRadius, sampledTheta] = internal::mapToRadiusTheta(mappedSample);
    const real cosTheta = std::cos(sampledTheta);
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    return
    {
        sampledRadius * cosTheta,
        sampledRadius * sinTheta
    };
}

} // namespace cadise