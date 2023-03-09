#include "core/instantiator/instantiator.h"

// estimator type
#include "core/estimator/aov/aoEstimator.h"
#include "core/estimator/aov/uvwEstimator.h"
#include "core/estimator/aov/wsnEstimator.h"
#include "core/estimator/ptEstimator.h"
#include "core/estimator/vptEstimator.h"
#include "core/estimator/wdlEstimator.h"

#include "file-io/scene-description/CSDResource.h"
#include "fundamental/assertion.h"

namespace cadise::instantiator 
{

static constexpr int32 MAX_PATH_LENGTH = 10000;

static std::shared_ptr<EnergyEstimator> createWdl(
    const std::shared_ptr<CSDResource>& data) 
{
    const int32 maxDepth = data->findInt<int32>("max-depth", MAX_PATH_LENGTH);

    return std::make_shared<WdlEstimator>(maxDepth);
}

static std::shared_ptr<EnergyEstimator> createVpt(
    const std::shared_ptr<CSDResource>& data) 
{
    const int32 maxDepth = data->findInt<int32>("max-depth", MAX_PATH_LENGTH);

    return std::make_shared<VptEstimator>(maxDepth);
}

static std::shared_ptr<EnergyEstimator> createPt(
    const std::shared_ptr<CSDResource>& data)
{
    const int32 maxDepth = data->findInt<int32>("max-path-length", MAX_PATH_LENGTH);

    return std::make_shared<PtEstimator>(maxDepth);
}

static std::shared_ptr<EnergyEstimator> createAovAo(
    const std::shared_ptr<CSDResource>& data)
{
    const std::size_t numSampleRays = data->findInt<std::size_t>("num-sample-rays", 4);

    return std::make_shared<AoEstimator>(numSampleRays);
}

static std::shared_ptr<EnergyEstimator> createAovUvw(
    const std::shared_ptr<CSDResource>& data) 
{
    return std::make_shared<UvwEstimator>();
}

static std::shared_ptr<EnergyEstimator> createAovWsn(
    const std::shared_ptr<CSDResource>& data)
{
    return std::make_shared<WsnEstimator>();
}

std::shared_ptr<EnergyEstimator> makeEstimator(
    const std::shared_ptr<CSDResource>& data) 
{
    CS_ASSERT(data);

    std::shared_ptr<EnergyEstimator> estimator = nullptr;

    const auto type = data->findString("estimator");
    if (type == "wdl") 
    {
        estimator = createWdl(data);
    }
    else if (type == "vpt") 
    {
        estimator = createVpt(data);
    }
    else if (type == "pt")
    {
        estimator = createPt(data);
    }
    else if (type == "aov-ao") 
    {
        estimator = createAovAo(data);
    }
    else if (type == "aov-uvw")
    {
        estimator = createAovUvw(data);
    }
    else if (type == "aov-wsn") 
    {
        estimator = createAovWsn(data);
    }
    else
    {
        // unsupported estimator type
        std::cerr << "Unsupported estimator type: <" << type << ">" << std::endl;
    }

    return estimator;
}

} // namespace cadise::instantiator