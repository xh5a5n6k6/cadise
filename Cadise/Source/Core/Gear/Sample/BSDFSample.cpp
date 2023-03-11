#include "Core/Gear/Sample/BSDFSample.h"

#include "Foundation/Assertion.h"

namespace cadise
{

BSDFSample::BSDFSample() :
    _scatterValue(0.0_r),
    _scatterDirection(0.0_r),
    _pdfW(0.0_r)
{}

const Spectrum& BSDFSample::scatterValue() const
{
    return _scatterValue;
}

const Vector3R& BSDFSample::scatterDirection() const
{
    return _scatterDirection;
}

real BSDFSample::pdfW() const
{
    return _pdfW;
}

void BSDFSample::setScatterValue(const Spectrum& scatterValue)
{
    _scatterValue = scatterValue;
}

void BSDFSample::setScatterDirection(const Vector3R& scatterDirection)
{
    CS_ASSERT(!scatterDirection.isZero());

    _scatterDirection = scatterDirection;
}

void BSDFSample::setPdfW(const real pdfW)
{
    _pdfW = pdfW;
}

bool BSDFSample::isValid() const
{
    return !_scatterValue.isZero() && _pdfW > 0.0_r;
}

} // namespace cadise