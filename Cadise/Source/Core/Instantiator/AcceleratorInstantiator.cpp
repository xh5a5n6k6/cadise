#include "Core/Instantiator/Instantiator.h"

// accelerator type
#include "Core/Intersector/Accelerator/BruteForceAccelerator.h"
#include "Core/Intersector/Accelerator/BVH/BVHAccelerator.h"
#include "Core/Intersector/Accelerator/KDTree/KDTreeAccelerator.h"

#include "Core/Intersector/Accelerator/BVH/EBVHSplitMode.h"
#include "FileIO/CSD/CSDResource.h"
#include "Foundation/Assertion.h"

namespace cadise::instantiator
{

static std::shared_ptr<Accelerator> createBruteForce(
    const std::shared_ptr<CSDResource>&              data,
    const std::vector<std::shared_ptr<Intersector>>& intersectors)
{
    return std::make_shared<BruteForceAccelerator>(std::move(intersectors));
}

static std::shared_ptr<Accelerator> createBvh(
    const std::shared_ptr<CSDResource>&              data,
    const std::vector<std::shared_ptr<Intersector>>& intersectors)
{
    const auto splitMode = data->findString("split-mode", "sah");

    EBVHSplitMode mode;
    if (splitMode == "equal")
    {
        mode = EBVHSplitMode::EqualCounts;
    }
    else if (splitMode == "sah")
    {
        mode = EBVHSplitMode::SAH;
    }
    else
    {
        mode = EBVHSplitMode::SAH;
    }

    return std::make_shared<BvhAccelerator>(std::move(intersectors), mode);
}

static std::shared_ptr<Accelerator> createKdTree(
    const std::shared_ptr<CSDResource>& data,
    const std::vector<std::shared_ptr<Intersector>>& intersectors)
{
    const real traversalCost    = data->findFloat<real>("traversal-cost", 1.0_r);
    const real intersectionCost = data->findFloat<real>("intersection-cost", 80.0_r);
    const real emptyBonus       = data->findFloat<real>("empty-bonus", 0.05_r);

    return std::make_shared<KDTreeAccelerator>(
        std::move(intersectors),
        traversalCost,
        intersectionCost,
        emptyBonus);
}

std::shared_ptr<Accelerator> makeAccelerator(
    const std::shared_ptr<CSDResource>& data,
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