#pragma once

#include "core/texture/category/tImageTexture.h"

#include "core/spectrum/spectrum.h"

namespace cadise {

class RgbaImageTexture : public TImageTexture<Spectrum, real, 4> {
public:
    using Parent = TImageTexture<Spectrum, real, 4>;
    using Parent::Parent;

    void evaluate(const Vector3R& uvw, Spectrum* const out_value) const override;
};

} // namespace cadise