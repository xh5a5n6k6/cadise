#include "core/material/matte.h"

#include "core/bxdf/lambertianBrdf.h"

namespace cadise {

Matte::Matte(const Vector3R albedo) {
    _addBxDF(std::make_shared<LambertianBRDF>(albedo));
}

} // namespace cadise