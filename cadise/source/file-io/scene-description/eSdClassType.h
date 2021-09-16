#pragma once

namespace cadise 
{

enum class ESdClassType 
{
    NONE,

    // global setting
    FILM,
    CAMERA,
    RENDERER,
    ACCELERATOR,
    LIGHT_CLUSTER,

    // world setting
    TEXTURE_REAL,
    TEXTURE_SPECTRUM,
    MATERIAL,
    LIGHT,
    PRIMITIVE,
};

} // namespace cadise