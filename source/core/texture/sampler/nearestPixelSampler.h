#pragma once

#include "core/texture/sampler/pixelSampler.h"

namespace cadise {

template<typename T, std::size_t N>
class NearestPixelSampler : public PixelSampler<T, N> {
public:
    using Parent = PixelSampler<T, N>;
    using Parent::Parent;
    
    /*
        When it needs to use template parent's attribute, it need to tell 
        the compiler explicitly that the names are in fact dependent on 
        the instantiation of the parent.

        Reference Note:
        https://stackoverflow.com/questions/6592512/templates-parent-class-member-variables-not-visible-in-inherited-class
    */
    using Parent::_uvwWrapper;

    void sample(
        const Vector3R& uvw,
        const Image<T, N>& image,
        Vector<T, N>* const out_value) const override;
};

} // namespace cadise

#include "core/texture/sampler/nearestPixelSampler.ipp"