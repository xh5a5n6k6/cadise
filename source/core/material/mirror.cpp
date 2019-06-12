#include "core/material/mirror.h"

#include "core/bxdf/specularBrdf.h"

namespace cadise {

Mirror::Mirror(const Vector3R albedo) {
    _addBxDF(std::make_shared<SpecularBRDF>(albedo));
}

} // namespace cadise