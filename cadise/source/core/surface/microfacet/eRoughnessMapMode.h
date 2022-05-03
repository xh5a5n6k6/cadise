#pragma once

namespace cadise 
{

/*! @brief Map from user-control roughness (0~1) to alpha.
*/
enum class ERoughnessMapMode 
{
    Undefined = 0,

    Square,
    PBRT,
};

} // namespace cadise