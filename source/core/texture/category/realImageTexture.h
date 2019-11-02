#pragma once

#include "core/texture/category/imageTexture.h"

namespace cadise {

class RealImageTexture : public ImageTexture<real, real, 3> {
public:
    using Parent = ImageTexture<real, real, 3>;
    using Parent::Parent;
    
    void evaluate(const Vector3R& uvw, real* const out_value) const override;

};

} // namespace cadise