#include "core/material/mirror.h"

#include "core/bxdf/specularBrdf.h"

namespace cadise {

Mirror::Mirror(Vector3F albedo) {
    _addBxDF(std::make_shared<SpecularBRDF>(albedo));
}

} // namespace cadise