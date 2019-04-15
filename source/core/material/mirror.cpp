#include "core/material/mirror.h"

#include "core/bxdf/specularBrdf.h"

namespace cadise {

Mirror::Mirror() {
    _addBxDF(std::make_shared<SpecularBRDF>());
}

} // namespace cadise