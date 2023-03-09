#pragma once

namespace cadise 
{

/*! @brief Scene description class type.
*/
enum class ESdClassType 
{
    Undefined = 0,

    // global setting
    Film,
    Camera,
    Renderer,
    Accelerator,
    LightCluster,

    // world setting
    TextureReal,
    TextureSpectrum,
    Material,
    Light,
    Primitive,
};

} // namespace cadise