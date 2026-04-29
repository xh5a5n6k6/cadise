#include "Core/Ray.h"

#include "Foundation/Assertion.h"
#include "Math/Constant.h"

#include <limits>

namespace cadise
{

Ray::Ray() :
    Ray(Vector3R(0.0_r), Vector3R(0.0_r, 0.0_r, -1.0_r))
{}

Ray::Ray(const Vector3R& origin, const Vector3R& direction) :
    Ray(origin, direction, constant::ray_epsilon<real>, std::numeric_limits<real>::max())
{}

Ray::Ray(const Vector3R& origin, const Vector3R& direction, const real minT, const real maxT) :
    _origin(origin),
    _direction(direction.normalize()),
    _minT(minT),
    _maxT(maxT)
{}

Vector3R Ray::at(const real t) const
{
    return _origin.add(_direction.mul(t));
}

void Ray::reset()
{
    this->setOrigin(Vector3R(0.0_r));
    this->setDirection(Vector3R(0.0_r, 0.0_r, -1.0_r));
    this->setMinT(constant::ray_epsilon<real>);
    this->setMaxT(std::numeric_limits<real>::max());
}

const Vector3R& Ray::origin() const
{
    return _origin;
}

const Vector3R& Ray::direction() const
{
    return _direction;
}

real Ray::minT() const
{
    return _minT;
}

real Ray::maxT() const
{
    return _maxT;
}

void Ray::setOrigin(const Vector3R& origin)
{
    _origin = origin;
}

void Ray::setDirection(const Vector3R& direction)
{
    CS_ASSERT(!direction.isZero());

    _direction = direction.normalize();
}

void Ray::setMinT(const real minT)
{
    _minT = minT;
}

void Ray::setMaxT(const real maxT)
{
    _maxT = maxT;
}

} // namespace cadise