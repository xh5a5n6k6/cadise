#include "core/material/matte.h"

#include "core/bxdf/lambertianBrdf.h"

namespace cadise {

Matte::Matte(Vector3 albedo) {
    _addBxDF(std::make_shared<LambertianBRDF>(albedo));
}

} // namespace cadise