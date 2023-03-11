#include "Core/Instantiator/Instantiator.h"

// estimator type
#include "Core/Estimator/AOV/AOEstimator.h"
#include "Core/Estimator/AOV/UVWEstimator.h"
#include "Core/Estimator/AOV/WSNEstimator.h"
#include "Core/Estimator/PTEstimator.h"
#include "Core/Estimator/VPTEstimator.h"
#include "Core/Estimator/WDLEstimator.h"

#include "FileIO/CSD/CSDResource.h"
#include "Foundation/Assertion.h"

namespace cadise::instantiator 
{

static constexpr int32 MAX_PATH_LENGTH = 10000;

static std::shared_ptr<EnergyEstimator> createWdl(
    const std::shared_ptr<CSDResource>& data) 
{
    const int32 maxDepth = data->findInt<int32>("max-depth", MAX_PATH_LENGTH);

    return std::make_shared<WDLEstimator>(maxDepth);
}

static std::shared_ptr<EnergyEstimator> createVpt(
    const std::shared_ptr<CSDResource>& data) 
{
    const int32 maxDepth = data->findInt<int32>("max-depth", MAX_PATH_LENGTH);

    return std::make_shared<VPTEstimator>(maxDepth);
}

static std::shared_ptr<EnergyEstimator> createPt(
    const std::shared_ptr<CSDResource>& data)
{
    const int32 maxDepth = data->findInt<int32>("max-path-length", MAX_PATH_LENGTH);

    return std::make_shared<PTEstimator>(maxDepth);
}

static std::shared_ptr<EnergyEstimator> createAovAo(
    const std::shared_ptr<CSDResource>& data)
{
    const std::size_t numSampleRays = data->findInt<std::size_t>("num-sample-rays", 4);

    return std::make_shared<AOEstimator>(numSampleRays);
}

static std::shared_ptr<EnergyEstimator> createAovUvw(
    const std::shared_ptr<CSDResource>& data) 
{
    return std::make_shared<UVWEstimator>();
}

static std::shared_ptr<EnergyEstimator> createAovWsn(
    const std::shared_ptr<CSDResource>& data)
{
    return std::make_shared<WSNEstimator>();
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