#pragma once

#include "Core/Texture/Mapper/TextureMapper.h"

namespace cadise
{

class SphericalMapper : public TextureMapper
{
public:
    SphericalMapper();

    void mappingToUvw(
        const Vector3R& direction, 
        Vector3R* const out_uvw) const override;
};

} // namespace cadise