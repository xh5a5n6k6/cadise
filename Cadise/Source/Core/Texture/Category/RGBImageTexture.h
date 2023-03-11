#pragma once

#include "Core/Texture/Category/TImageTexture.h"

#include "Core/Spectrum/Spectrum.h"

namespace cadise 
{

class RGBImageTexture : public TImageTexture<Spectrum, real, 3>
{
public:
    using Parent = TImageTexture<Spectrum, real, 3>;
    using Parent::Parent;

    void evaluate(const Vector3R& uvw, Spectrum* const out_value) const override;
};

} // namespace cadise