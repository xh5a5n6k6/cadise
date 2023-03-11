#include "Core/Gear/Sample/DirectLightSample.h"

namespace cadise 
{

DirectLightSample::DirectLightSample() :
    _targetPosition(),
    _emitPosition(),
    _emitNormal(),
    _radiance(0.0_r),
    _pdfW(0.0_r) 
{}

const Vector3R& DirectLightSample::targetPosition() const 
{
    return _targetPosition;
}

const Vector3R& DirectLightSample::emitPosition() const 
{
    return _emitPosition;
}

const Vector3R& DirectLightSample::emitNormal() const 
{
    return _emitNormal;
}

const Spectrum& DirectLightSample::radiance() const 
{
    return _radiance;
}

real DirectLightSample::pdfW() const 
{
    return _pdfW;
}

void DirectLightSample::setTargetPosition(const Vector3R& targetPosition) 
{
    _targetPosition = targetPosition;
}

void DirectLightSample::setEmitPosition(const Vector3R& emitPosition) 
{
    _emitPosition = emitPosition;
}

void DirectLightSample::setEmitNormal(const Vector3R& emitNormal)
{
    _emitNormal = emitNormal;
}

void DirectLightSample::setRadiance(const Spectrum& radiance)
{
    _radiance = radiance;
}

void DirectLightSample::setPdfW(const real pdfW) 
{
    _pdfW = pdfW;
}

bool DirectLightSample::isValid() const 
{
    return !_radiance.isZero() && _pdfW > 0.0_r;
}

} // namespace cadise