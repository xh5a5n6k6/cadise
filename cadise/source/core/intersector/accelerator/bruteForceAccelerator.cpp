#include "core/intersector/accelerator/bruteForceAccelerator.h"

#include "fundamental/assertion.h"
#include "math/tAabb3.h"

namespace cadise 
{

BruteForceAccelerator::BruteForceAccelerator(const std::vector<std::shared_ptr<Intersector>>& intersectors) :
    Accelerator(),
    _intersectors(std::move(intersectors))
{}

void BruteForceAccelerator::evaluateBound(AABB3R* const out_bound) const 
{
    CS_ASSERT(out_bound);

    AABB3R bound;
    AABB3R tmpBound;
    for (std::size_t i = 0; i < _intersectors.size(); ++i) 
    {
        _intersectors[i]->evaluateBound(&tmpBound);
        bound.unionWithLocal(tmpBound);
    }

    out_bound->set(bound);
}

bool BruteForceAccelerator::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const 
{
    bool result = false;
    for (std::size_t i = 0; i < _intersectors.size(); ++i)
    {
        result |= _intersectors[i]->isIntersecting(ray, primitiveInfo);
    }

    return result;
}

bool BruteForceAccelerator::isOccluded(const Ray& ray) const 
{
    for (std::size_t i = 0; i < _intersectors.size(); ++i) 
    {
        if (_intersectors[i]->isOccluded(ray))
        {
            return true;
        }
    }

    return false;
}

} // namespace cadise