#include "core/instantiator/instantiator.h"

// sampler type
#include "core/sampler/category/randomSampler.h"
#include "core/sampler/category/stratifiedSampler.h"

#include "file-io/scene-description/CSDResource.h"
#include "fundamental/assertion.h"

namespace cadise::instantiator 
{

static std::shared_ptr<Sampler> createRandom(
    const std::shared_ptr<CSDResource>& data) 
{
    const std::size_t sampleNumber = data->findInt<std::size_t>("sample-number", 4);

    return std::make_shared<RandomSampler>(sampleNumber);
}

static std::shared_ptr<Sampler> createStratified(
    const std::shared_ptr<CSDResource>& data)
{
    const std::size_t sampleNumber = data->findInt<std::size_t>("sample-number", 4);

    return std::make_shared<StratifiedSampler>(sampleNumber);
}

std::shared_ptr<Sampler> makeSampler(
    const std::shared_ptr<CSDResource>& data) 
{
    CS_ASSERT(data);

    std::shared_ptr<Sampler> sampler = nullptr;
    
    const auto type = data->findString("sampler");
    if (type == "random") 
    {
        sampler = createRandom(data);
    }
    else if (type == "stratified")
    {
        sampler = createStratified(data);
    }
    else
    {
        // unsupported sampler type
    }

    return sampler;
}

} // namespace cadise::instantiator