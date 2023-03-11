#pragma once

namespace cadise 
{

/*! @brief Weighting function used in Multiple Importance Sampling (MIS).
*/
enum class EMISMode
{
    Undefined = 0,

    /*! Balance heuristic weighting function. */
    Balance,

    /*! Power heuristic weighting function. */
    Power,
};

} // namespace cadise