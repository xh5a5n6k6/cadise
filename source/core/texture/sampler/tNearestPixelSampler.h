#pragma once

#include "core/texture/sampler/tPixelSampler.h"

namespace cadise {

template<typename T, std::size_t N>
class TNearestPixelSampler : public TPixelSampler<T, N> {
public:
    using Parent = TPixelSampler<T, N>;
    using Parent::Parent;
    
    /*
        When it needs to use template parent's attribute, it need to tell 
        the compiler explicitly that the names are in fact dependent on 
        the instantiation of the parent.

        Reference Note:
        https://stackoverflow.com/questions/6592512/templates-parent-class-member-variables-not-visible-in-inherited-class
    */
    using Parent::_image;
    using Parent::_uvwWrapper;

    void sample(
        const Vector3R&               uvw,
        TArithmeticArray<T, N>* const out_value) const override;
};

} // namespace cadise

#include "core/texture/sampler/tNearestPixelSampler.ipp"