#include "core/material/blinnPhong.h"

#include "core/bxdf/blinnPhongBrdf.h"

namespace cadise {

BlinnPhong::BlinnPhong(Vector3 albedo, float diffuseRatio, float exponent){
    std::shared_ptr<BxDF> bxdf = std::make_shared<BlinnPhongBRDF>(albedo, diffuseRatio, exponent);
    _addBxDF(bxdf);
}

} // namespace cadise