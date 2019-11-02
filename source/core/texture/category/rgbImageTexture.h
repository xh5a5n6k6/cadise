#pragma once

#include "core/texture/category/imageTexture.h"

#include "core/spectrum/spectrum.h"

namespace cadise {

class RgbImageTexture : public ImageTexture<Spectrum, real, 3> {
public:
    using Parent = ImageTexture<Spectrum, real, 3>;
    using Parent::Parent;

    void evaluate(const Vector3R& uvw, Spectrum* const out_value) const override;
};

} // namespace cadise