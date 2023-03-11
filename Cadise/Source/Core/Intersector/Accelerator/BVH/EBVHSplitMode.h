#pragma once

namespace cadise 
{

/*! @brief Split strategy used in BVH construction.
*/
enum class EBVHSplitMode 
{
    Undefined = 0,

    /*! Split objects into two equal-sized subsets. */
    EqualCounts,

    /*! Split objects based on the Surface Area Heuristic (SAH) model. */
    SAH,
};

} // namespace cadise