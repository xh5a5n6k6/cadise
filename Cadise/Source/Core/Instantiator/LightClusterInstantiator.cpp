#include "Core/Instantiator/Instantiator.h"

// lightCluster type
#include "Core/LightCluster/PowerLightCluster.h"
#include "Core/LightCluster/UniformLightCluster.h"

#include "FileIO/CSD/CSDResource.h"
#include "Foundation/Assertion.h"

namespace cadise::instantiator 
{

static std::shared_ptr<LightCluster> createUniform(
    const std::shared_ptr<CSDResource>&             data,
    const std::vector<std::shared_ptr<Light>>& lights)
{
    return std::make_shared<UniformLightCluster>(std::move(lights));
}

static std::shared_ptr<LightCluster> createPower(
    const std::shared_ptr<CSDResource>&             data,
    const std::vector<std::shared_ptr<Light>>& lights)
{
    return std::make_shared<PowerLightCluster>(std::move(lights));
}

std::shared_ptr<LightCluster> makeLightCluster(
    const std::shared_ptr<CSDResource>&             data,
    const std::vector<std::shared_ptr<Light>>& lights)
{
    CS_ASSERT(data);

    std::shared_ptr<LightCluster> lightCluster = nullptr;

    const auto type = data->findString("type");
    if (type == "uniform") 
    {
        lightCluster = createUniform(data, lights);
    }
    else if (type == "power") 
    {
        lightCluster = createPower(data, lights);
    }
    else
    {
        // unsupported lightCluster type
    }

    return lightCluster;
}

} // namespace cadise::instantiator