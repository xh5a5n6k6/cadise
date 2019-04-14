#pragma once

#include "core/bxdf/bxdf.h"

namespace cadise {

class BlinnPhongBRDF : public BxDF {
public:
    BlinnPhongBRDF(Vector3 albedo, float exponent = 32.0f, float diffuseRatio = 0.7f);

    Vector3 evaluate(Vector3 &inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    Vector3 _albedo;
    float _exponent;
    float _diffuseRatio;
    float _brdfFactor;
};

} // namespace cadise