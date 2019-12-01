#include "core/instantiator/instantiator.h"

// sampler type
#include "core/sampler/category/randomSampler.h"
#include "core/sampler/category/stratifiedSampler.h"

#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Sampler> createRandom(
    const std::shared_ptr<SdData>& data) {

    const int32 sampleNumber = data->findInt32("sample-number", 4);

    return std::make_shared<RandomSampler>(static_cast<std::size_t>(sampleNumber));
}

static std::shared_ptr<Sampler> createStratified(
    const std::shared_ptr<SdData>& data) {

    const int32 sampleNumber = data->findInt32("sample-number", 4);

    return std::make_shared<StratifiedSampler>(static_cast<std::size_t>(sampleNumber));
}

std::shared_ptr<Sampler> makeSampler(
    const std::shared_ptr<SdData>& data) {

    CADISE_ASSERT(data);

    std::shared_ptr<Sampler> sampler = nullptr;
    
    const std::string_view type = data->findString("sampler");
    if (type == "random") {
        sampler = createRandom(data);
    }
    else if (type == "stratified") {
        sampler = createStratified(data);
    }
    else {
        // don't support sampler type
    }

    return sampler;
}

} // namespace instantiator

} // namespace cadise