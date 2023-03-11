#pragma once

#include "Math/Type/MathType.h"

#include <vector>

// forward declaration
namespace cadise
{
    class Camera;
    class PMViewpoint;
    class Scene;
}

namespace cadise
{

class PMViewpointConstructor 
{
public:
    PMViewpointConstructor(
        const Scene* const  scene,
        const Camera* const camera,
        const real          initialRadius);

    void construct(
        const Vector2D&                 filmPosition,
        std::vector<PMViewpoint>* const out_viewpoints) const;

private:
    const Scene*  _scene;
    const Camera* _camera;

    real          _initialRadius;
};

} // namespace cadise