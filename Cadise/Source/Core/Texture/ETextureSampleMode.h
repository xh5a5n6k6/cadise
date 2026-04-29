#pragma once

namespace cadise
{

/*! @brief Resampling method used in image interpolation.
*/
enum class ETextureSampleMode
{
    Undefined = 0,

    /*! Choose the value of the point which is closest to the current sample. */
    Nearest,

    // TODO:
    // BILINEAR,
};

} // namespace cadise