#pragma once

#include "core/surfaceIntersection.h"
#include "core/texture/tTexture.h"

namespace cadise {

template<typename T>
class TSurfaceSampler {
public:
    void sample(
        const SurfaceIntersection& si,
        const TTexture<T>* const   texture,
        T* const                   out_value) const;
};

// template header implementation

template<typename T>
inline void TSurfaceSampler<T>::sample(
    const SurfaceIntersection& si,
    const TTexture<T>* const   texture,
    T* const                   out_value) const {

    CADISE_ASSERT(texture);
    CADISE_ASSERT(out_value);

    const Vector3R& uvw = si.surfaceDetail().uvw();
    texture->evaluate(uvw, out_value);
}

} // namespace cadise