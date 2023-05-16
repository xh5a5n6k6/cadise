#pragma once

#include "Core/Renderer/RenderWork/TileWork.h"

// forward declaration
namespace cadise
{
    class Camera;
    class EnergyEstimator;
    class Sampler;
    class Scene;
}

namespace cadise
{

class EstimatorTileWork : public TileWork
{
public:
    EstimatorTileWork(
        const Scene* const           scene,
        const Camera* const          camera,
        const EnergyEstimator* const estimator,
        const Sampler* const         sampler);

    void work() const override;

private:
    const Scene*           _scene;
    const Camera*          _camera;
    const EnergyEstimator* _estimator;
    const Sampler*         _sampler;
};

} // namespace cadise