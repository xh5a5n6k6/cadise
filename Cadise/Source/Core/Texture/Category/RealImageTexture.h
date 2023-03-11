#pragma once

#include "Core/Texture/Category/TImageTexture.h"

namespace cadise
{

class RealImageTexture : public TImageTexture<real, real, 3>
{
public:
    using Parent = TImageTexture<real, real, 3>;
    using Parent::Parent;
    
    void evaluate(const Vector3R& uvw, real* const out_value) const override;

};

} // namespace cadise