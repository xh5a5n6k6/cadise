#include "core/instantiator/instantiator.h"

// accelerator type
#include "core/intersector/accelerator/bruteForceAccelerator.h"
#include "core/intersector/accelerator/bvh/bvhAccelerator.h"
#include "core/intersector/accelerator/bvh/bvhSplitMode.h"
#include "core/intersector/accelerator/kd-tree/kdTreeAccelerator.h"

#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Accelerator> createBruteForce(
    const std::shared_ptr<SdData>& data,
    const std::vector<std::shared_ptr<Intersector>>& intersectors) {

    return std::make_shared<BruteForceAccelerator>(std::move(intersectors));
}

static std::shared_ptr<Accelerator> createBvh(
    const std::shared_ptr<SdData>& data,
    const std::vector<std::shared_ptr<Intersector>>& intersectors) {

    const std::string_view splitMode = data->findString("split-mode", "sah");

    BvhSplitMode mode;
    if (splitMode == "equal") {
        mode = BvhSplitMode::EQUAL;
    }
    else if (splitMode == "sah") {
        mode = BvhSplitMode::SAH;
    }
    else {
        mode = BvhSplitMode::SAH;
    }

    return std::make_shared<BvhAccelerator>(std::move(intersectors), mode);
}

static std::shared_ptr<Accelerator> createKdTree(
    const std::shared_ptr<SdData>& data,
    const std::vector<std::shared_ptr<Intersector>>& intersectors) {

    const real traversalCost    = data->findReal("traversal-cost", 1.0_r);
    const real intersectionCost = data->findReal("intersection-cost", 80.0_r);
    const real emptyBonus       = data->findReal("empty-bonus", 0.05_r);

    return std::make_shared<KdTreeAccelerator>(std::move(intersectors),
                                               traversalCost,
                                               intersectionCost,
                                               emptyBonus);
}

std::shared_ptr<Accelerator> makeAccelerator(
    const std::shared_ptr<SdData>& data,
    const std::vector<std::shared_ptr<Intersector>>& intersectors) {

    CADISE_ASSERT(data);

    std::shared_ptr<Accelerator> accelerator = nullptr;
    
    const std::string_view type = data->findString("type");
    if (type == "bruteForce") {
        accelerator = createBruteForce(data, intersectors);
    }
    else if (type == "bvh") {
        accelerator = createBvh(data, intersectors);
    }
    else if (type == "kd-tree") {
        accelerator = createKdTree(data, intersectors);
    }
    else {
        // don't support primitive type
    }

    return accelerator;
}

} // namespace instantiator

} // namespace cadise