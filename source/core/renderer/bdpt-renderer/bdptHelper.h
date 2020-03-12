#pragma once

#include "math/type/fundamentalType.h"

namespace cadise {

class Scene;
class PathVertex;

class BdptHelper {
public:
    static bool canConnect(
        const Scene&      scene,
        const PathVertex& lightEndpoint, 
        const PathVertex& cameraEndpoint,
        real* const       out_connectG);
};

} // namespace cadise