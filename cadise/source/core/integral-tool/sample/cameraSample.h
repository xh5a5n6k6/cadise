#pragma once

#include "core/spectrum/spectrum.h"
#include "math/tVector2.h"
#include "math/tVector3.h"

namespace cadise 
{

class CameraSample 
{
public:
    CameraSample();

    const Vector3R& targetPosition() const;
    const Vector3R& cameraPosition() const;
    const Vector3R& cameraNormal() const;
    const Vector2D& filmPosition() const;
    const Spectrum& importance() const;
    real pdfW() const;

    void setTargetPosition(const Vector3R& targetPosition);
    void setCameraPosition(const Vector3R& cameraPosition);
    void setCameraNormal(const Vector3R& cameraNormal);
    void setFilmPosition(const Vector2D& filmPosition);
    void setImportance(const Spectrum& importance);
    void setPdfW(const real pdfW);

    bool isValid() const;

private:
    Vector3R _targetPosition;
    Vector3R _cameraPosition;
    Vector3R _cameraNormal;
    Vector2D _filmPosition;
    Spectrum _importance;
    real     _pdfW;
};

} // namespace cadise