#pragma once

namespace cadise {

enum class SdClassType {
    NONE,

    // global setting
    FILM,
    CAMERA,
    RENDERER,
    ACCELERATOR,

    // world setting
    TEXTURE_REAL,
    TEXTURE_SPECTRUM,
    MATERIAL,
    LIGHT,
    PRIMITIVE,
};

} // namespace cadise