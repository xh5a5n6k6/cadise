#include "core/instantiator/instantiator.h"

// integrator type
#include "core/integrator/whittedIntegrator.h"

#include "file-io/scene-description/sdData.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Integrator> createWhitted(
    const std::shared_ptr<SdData>& data) {

    const int32 maxDepth = data->findInt32("max-depth", 4);

    return std::make_shared<WhittedIntegrator>(maxDepth);
}

std::shared_ptr<Integrator> makeIntegrator(
    const std::shared_ptr<SdData>& data) {

    std::shared_ptr<Integrator> integrator = nullptr;
    std::string_view type = data->findString("integrator");
    if (!type.compare("whitted")) {
        integrator = createWhitted(data);
    }
    else {
        // don't support integrator type
    }

    return integrator;
}

} // namespace instantiator

} // namespace cadise