#pragma once

#include "math/tVector3.h"

namespace cadise {

class PositionSample {
public:
    PositionSample();

    const Vector3R& position() const;
    const Vector3R& geometryNormal() const;
    const Vector3R& shadingNormal() const;
    const Vector3R& uvw() const;
    real pdfA() const;

    void setPosition(const Vector3R& position);
    void setGeometryNormal(const Vector3R& geometryNormal);
    void setShadingNormal(const Vector3R& shadingNormal);
    void setUvw(const Vector3R& uvw);
    void setPdfA(const real pdfA);

    bool isValid() const;

private:
    Vector3R _position;
    Vector3R _geometryNormal;
    Vector3R _shadingNormal;
    Vector3R _uvw;
    real     _pdfA;
};

} // namespace cadise