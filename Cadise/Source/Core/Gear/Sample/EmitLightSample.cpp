#include "Core/Gear/Sample/EmitLightSample.h"

namespace cadise
{

EmitLightSample::EmitLightSample() :
    _emitPosition(),
    _emitDirection(),
    _emitNormal(),
    _radiance(0.0_r),
    _pdfA(0.0_r),
    _pdfW(0.0_r)
{}

const Vector3R& EmitLightSample::emitPosition() const
{
    return _emitPosition;
}

const Vector3R& EmitLightSample::emitDirection() const
{
    return _emitDirection;
}

const Vector3R& EmitLightSample::emitNormal() const
{
    return _emitNormal;
}

const Spectrum& EmitLightSample::radiance() const
{
    return _radiance;
}

real EmitLightSample::pdfA() const
{
    return _pdfA;
}

real EmitLightSample::pdfW() const
{
    return _pdfW;
}

void EmitLightSample::setEmitPosition(const Vector3R& emitPosition)
{
    _emitPosition = emitPosition;
}

void EmitLightSample::setEmitDirection(const Vector3R& emitDirection)
{
    _emitDirection = emitDirection;
}

void EmitLightSample::setEmitNormal(const Vector3R& emitNormal)
{
    _emitNormal = emitNormal;
}

void EmitLightSample::setRadiance(const Spectrum& radiance)
{
    _radiance = radiance;
}

void EmitLightSample::setPdfA(const real pdfA)
{
    _pdfA = pdfA;
}

void EmitLightSample::setPdfW(const real pdfW)
{
    _pdfW = pdfW;
}

bool EmitLightSample::isValid() const
{
    return !_radiance.isZero() && _pdfA > 0.0_r && _pdfW > 0.0_r;
}

} // namespace cadise