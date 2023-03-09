#pragma once

#include "math/type/fundamentalType.h"

namespace cadise { class Scene; }
namespace cadise { class SubPath; }

namespace cadise
{

class BdptMis
{
public:
    static real weight(
        const Scene&      scene,
        const SubPath&    lightPath,
        const SubPath&    cameraPath,
        const std::size_t s,
        const std::size_t t);
};

} // namespace cadise