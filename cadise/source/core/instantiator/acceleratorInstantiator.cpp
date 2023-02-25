#include "core/instantiator/instantiator.h"

// accelerator type
#include "core/intersector/accelerator/bruteForceAccelerator.h"
#include "core/intersector/accelerator/bvh/bvhAccelerator.h"
#include "core/intersector/accelerator/kd-tree/kdTreeAccelerator.h"

#include "core/intersector/accelerator/bvh/eBvhSplitMode.h"
#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

namespace cadise::instantiator
{

static std::shared_ptr<Accelerator> createBruteForce(
    const std::shared_ptr<SdData>& data,
    const std::vector<std::shared_ptr<Intersector>>& intersectors)
{
    return std::make_shared<BruteForceAccelerator>(std::move(intersectors));
}

static std::shared_ptr<Accelerator> createBvh(
    const std::shared_ptr<SdData>& data,
    const std::vector<std::shared_ptr<Intersector>>& intersectors)
{
    const auto splitMode = data->findString("split-mode", "sah");

    EBvhSplitMode mode;
    if (splitMode == "equal")
    {
        mode = EBvhSplitMode::EqualCounts;
    }
    else if (splitMode == "sah")
    {
        mode = EBvhSplitMode::SAH;
    }
    else
    {
        mode = EBvhSplitMode::SAH;
    }

    return std::make_shared<BvhAccelerator>(std::move(intersectors), mode);
}

static std::shared_ptr<Accelerator> createKdTree(
    const std::shared_ptr<SdData>& data,
    const std::vector<std::shared_ptr<Intersector>>& intersectors)
{
    const real traversalCost    = data->findFloat<real>("traversal-cost", 1.0_r);
    const real intersectionCost = data->findFloat<real>("intersection-cost", 80.0_r);
    const real emptyBonus       = data->findFloat<real>("empty-bonus", 0.05_r);

    return std::make_shared<KdTreeAccelerator>(
        std::move(intersectors),
        traversalCost,
        intersectionCost,
        emptyBonus);
}

std::shared_ptr<Accelerator> makeAccelerator(
    const std::shared_ptr<SdData>& data,
    const std::vector<std::shared_ptr<Intersector>>& intersectors)
{
    CS_ASSERT(data);

    std::shared_ptr<Accelerator> accelerator = nullptr;
    
    const auto type = data->findString("type");
    if (type == "bruteForce") 
    {
        accelerator = createBruteForce(data, intersectors);
    }
    else if (type == "bvh") 
    {
        accelerator = createBvh(data, intersectors);
    }
    else if (type == "kd-tree") 
    {
        accelerator = createKdTree(data, intersectors);
    }
    else
    {
        // unsupported accelerator type
    }

    return accelerator;
}

} // namespace cadise::instantiator