#pragma once

#include "Core/Texture/Category/TImageTexture.h"

#include "Core/Spectrum/Spectrum.h"

namespace cadise
{

class RGBAImageTexture : public TImageTexture<Spectrum, real, 4> 
{
public:
    using Parent = TImageTexture<Spectrum, real, 4>;
    using Parent::Parent;

    void evaluate(const Vector3R& uvw, Spectrum* const out_value) const override;
};

} // namespace cadise