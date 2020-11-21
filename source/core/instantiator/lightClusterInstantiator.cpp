#include "instantiator.h"

// lightCluster type
#include "core/light/cluster/powerLightCluster.h"
#include "core/light/cluster/uniformLightCluster.h"

#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<LightCluster> createUniform(
    const std::shared_ptr<SdData>&             data,
    const std::vector<std::shared_ptr<Light>>& lights) {

    return std::make_shared<UniformLightCluster>(std::move(lights));
}

static std::shared_ptr<LightCluster> createPower(
    const std::shared_ptr<SdData>&             data,
    const std::vector<std::shared_ptr<Light>>& lights) {

    return std::make_shared<PowerLightCluster>(std::move(lights));
}

std::shared_ptr<LightCluster> makeLightCluster(
    const std::shared_ptr<SdData>&             data,
    const std::vector<std::shared_ptr<Light>>& lights) {

    CADISE_ASSERT(data);

    std::shared_ptr<LightCluster> lightCluster = nullptr;

    const auto type = data->findString("type");
    if (type == "uniform") {
        lightCluster = createUniform(data, lights);
    }
    else if (type == "power") {
        lightCluster = createPower(data, lights);
    }
    else {
        // unsupported lightCluster type
    }

    return lightCluster;
}

} // namespace instantiator

}