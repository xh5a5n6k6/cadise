#pragma once

namespace cadise 
{

/*! @brief Method to remap texture coordinates to the range from 0 to 1.
*/
enum class ETextureWrapMode 
{
    Undefined = 0,

    /*! Clamp the texture coordinate to the edge value (i.e. 0-1). */
    Clamp,

    /*!
    Map the texture coordinate to the repeating pattern by subtracting
    its floor number.
    */
    Repeat,
};

} // namespace cadise