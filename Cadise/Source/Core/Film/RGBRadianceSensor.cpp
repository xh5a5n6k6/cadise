#include "Core/Film/RGBRadianceSensor.h"

#include "Math/TVector3.h"

namespace cadise
{

RGBRadianceSensor::RGBRadianceSensor() :
    _r(0.0_r),
    _g(0.0_r),
    _b(0.0_r),
    _weight(0.0_r) 
{}

void RGBRadianceSensor::addValue(const Vector3R& value)
{
    this->addValue(value.x(), value.y(), value.z());
}

void RGBRadianceSensor::addValue(const real r, const real g, const real b)
{
    _r += r;
    _g += g;
    _b += b;
}

void RGBRadianceSensor::addWeight(const real weight)
{
    _weight += weight;
}

real RGBRadianceSensor::r() const
{
    return _r;
}

real RGBRadianceSensor::g() const 
{
    return _g;
}

real RGBRadianceSensor::b() const 
{
    return _b;
}

real RGBRadianceSensor::weight() const
{
    return _weight;
}

} // namespace cadise