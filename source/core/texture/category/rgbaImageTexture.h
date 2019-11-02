#pragma once

#include "core/texture/category/imageTexture.h"

#include "core/spectrum/spectrum.h"

namespace cadise {

class RgbaImageTexture : public ImageTexture<Spectrum, real, 4> {
public:
    using Parent = ImageTexture<Spectrum, real, 4>;
    using Parent::Parent;

    void evaluate(const Vector3R& uvw, Spectrum* const out_value) const override;
};

} // namespace cadise