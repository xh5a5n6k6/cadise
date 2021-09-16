#include "core/intersector/accelerator/bvh/bvhAccelerator.h"

#include "core/intersector/accelerator/bvh/bvhBinaryNode.h"
#include "core/intersector/accelerator/bvh/bvhBuilder.h"
#include "core/ray.h"
#include "fundamental/assertion.h"

namespace cadise 
{

BvhAccelerator::BvhAccelerator(
    const std::vector<std::shared_ptr<Intersector>>& intersectors,
    const EBvhSplitMode                              splitMode) :
    
    Accelerator(),
    _intersectors(),
    _nodes()
{
    _intersectors.reserve(intersectors.size());

    const BvhBuilder builder(splitMode);

    // build binary node tree recursively
    std::size_t totalNodeSize = 0;
    std::unique_ptr<BvhBinaryNode> root = builder.buildBinaryNodes(std::move(intersectors), 
                                                                   &_intersectors, 
                                                                   &totalNodeSize);

    // flatten the binary tree and use BVHLinearNode to store the informations
    _nodes.clear();
    _nodes.shrink_to_fit();
    builder.buildLinearNodes(std::move(root), totalNodeSize, &_nodes);
}

void BvhAccelerator::evaluateBound(AABB3R* const out_bound) const
{
    CADISE_ASSERT(out_bound);

    out_bound->set(_nodes[0].bound());
}

bool BvhAccelerator::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const 
{
    bool result = false;

    const Vector3R& origin                 = ray.origin();
    const Vector3R  inverseDirection       = ray.direction().reciprocal();
    const int32     directionIsNegative[3] = { 
        inverseDirection.x() < 0.0_r, 
        inverseDirection.y() < 0.0_r, 
        inverseDirection.z() < 0.0_r 
    };
    
    std::size_t currentNodeIndex = 0;
    std::size_t currentStackSize = 0;
    std::size_t nodeStack[MAX_STACK_SIZE];

    while (true)
    {
        const BvhLinearNode& currentNode = _nodes[currentNodeIndex];

        if (currentNode.bound().isIntersectingAABB(origin, inverseDirection, ray.minT(), ray.maxT())) 
        {
            if (currentNode.isLeaf()) 
            {
                for (std::size_t i = 0; i < currentNode.intersectorCounts(); ++i)
                {
                    result |= _intersectors[currentNode.intersectorIndex() + i]->isIntersecting(ray, primitiveInfo);
                }

                if (currentStackSize == 0) 
                {
                    break;
                }
                else 
                {
                    --currentStackSize;
                    currentNodeIndex = nodeStack[currentStackSize];
                }
            } // end leaf node

            else 
            {
                if (directionIsNegative[currentNode.splitAxis()]) 
                {
                    nodeStack[currentStackSize] = currentNodeIndex + 1;
                    ++currentStackSize;
                    currentNodeIndex = currentNode.secondChildIndex();
                }
                else
                {
                    nodeStack[currentStackSize] = currentNode.secondChildIndex();
                    ++currentStackSize;
                    currentNodeIndex = currentNodeIndex + 1;
                }
            } // end internal node
        } // end node intersection

        else 
        {
            if (currentStackSize == 0) 
            {
                break;
            }
            else 
            {
                --currentStackSize;
                currentNodeIndex = nodeStack[currentStackSize];
            }
        }
    } // end while loop

    return result;
}

bool BvhAccelerator::isOccluded(const Ray& ray) const
{
    const Vector3R& origin                 = ray.origin();
    const Vector3R  inverseDirection       = ray.direction().reciprocal();
    const int32     directionIsNegative[3] = { 
        inverseDirection.x() < 0.0_r,
        inverseDirection.y() < 0.0_r,
        inverseDirection.z() < 0.0_r 
    };

    std::size_t currentNodeIndex = 0;
    std::size_t currentStackSize = 0;
    std::size_t nodeStack[MAX_STACK_SIZE];

    while (true) 
    {
        const BvhLinearNode& currentNode = _nodes[currentNodeIndex];

        if (currentNode.bound().isIntersectingAABB(origin, inverseDirection, ray.minT(), ray.maxT())) 
        {
            if (currentNode.isLeaf()) 
            {
                for (std::size_t i = 0; i < currentNode.intersectorCounts(); ++i)
                {
                    if (_intersectors[currentNode.intersectorIndex() + i]->isOccluded(ray)) 
                    {
                        return true;
                    }
                }

                if (currentStackSize == 0) 
                {
                    break;
                }
                else 
                {
                    --currentStackSize;
                    currentNodeIndex = nodeStack[currentStackSize];
                }
            } // end leaf node

            else
            {
                if (directionIsNegative[currentNode.splitAxis()])
                {
                    nodeStack[currentStackSize] = currentNodeIndex + 1;
                    ++currentStackSize;
                    currentNodeIndex = currentNode.secondChildIndex();
                }
                else 
                {
                    nodeStack[currentStackSize] = currentNode.secondChildIndex();
                    ++currentStackSize;
                    currentNodeIndex = currentNodeIndex + 1;
                }
            } // end internal node
        } // end node intersection

        else
        {
            if (currentStackSize == 0) 
            {
                break;
            }
            else 
            {
                --currentStackSize;
                currentNodeIndex = nodeStack[currentStackSize];
            }
        }
    } // end while loop

    return false;
}

} // namespace cadise