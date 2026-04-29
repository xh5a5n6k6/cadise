#pragma once

#include "Math/Type/PrimitiveType.h"

// forward declaration
namespace cadise
{
    class Scene;
    class Subpath;
}

namespace cadise
{

class BDPTMIS
{
public:
    static real weight(
        const Scene&      scene,
        const Subpath&    lightPath,
        const Subpath&    cameraPath,
        const std::size_t s,
        const std::size_t t);
};

} // namespace cadise