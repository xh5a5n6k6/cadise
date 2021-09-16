#pragma once

#include "math/type/mathType.h"

#include <vector>

namespace cadise { class Camera; }
namespace cadise { class PmViewpoint; }
namespace cadise { class Scene; }

namespace cadise
{

class PmViewpointConstructor 
{
public:
    PmViewpointConstructor(
        const Scene* const  scene,
        const Camera* const camera,
        const real          initialRadius);

    void construct(
        const Vector2D&                 filmPosition,
        std::vector<PmViewpoint>* const out_viewpoints) const;

private:
    const Scene*  _scene;
    const Camera* _camera;

    real          _initialRadius;
};

} // namespace cadise