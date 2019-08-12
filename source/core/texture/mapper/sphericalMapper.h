#pragma once

#include "core/texture/mapper/textureMapper.h"

namespace cadise {

class SphericalMapper : public TextureMapper {
public:
    SphericalMapper();

    Vector3R mappingToUvw(const SurfaceInfo& surfaceInfo) const override;

private:

};

} // namespace cadise