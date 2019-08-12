#include "core/instantiator/instantiator.h"

// accelerator type
#include "core/intersector/accelerator/bruteForceAccelerator.h"
#include "core/intersector/accelerator/bvh/bvhAccelerator.h"

#include "file-io/scene-description/sdData.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Accelerator> createBruteForce(
    const std::shared_ptr<SdData>& data,
    const std::vector<std::shared_ptr<Intersector>> _intersectors) {

    return std::make_shared<BruteForceAccelerator>(std::move(_intersectors));
}

static std::shared_ptr<Accelerator> createBvh(
    const std::shared_ptr<SdData>& data,
    const std::vector<std::shared_ptr<Intersector>> _intersectors) {

    return std::make_shared<BVHAccelerator>(std::move(_intersectors));
}

std::shared_ptr<Accelerator> makeAccelerator(
    const std::shared_ptr<SdData>& data,
    const std::vector<std::shared_ptr<Intersector>> intersectors) {

    std::shared_ptr<Accelerator> accelerator = nullptr;
    std::string_view type = data->findString("type");
    if (!type.compare("bruteForce")) {
        accelerator = createBruteForce(data, intersectors);
    }
    else if (!type.compare("bvh")) {
        accelerator = createBvh(data, intersectors);
    }
    else {
        // don't support primitive type
    }

    return accelerator;
}

} // namespace instantiator

} // namespace cadise