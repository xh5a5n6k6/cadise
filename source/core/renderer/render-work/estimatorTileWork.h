#pragma once

#include "core/renderer/render-work/tileWork.h"

namespace cadise {

class Camera;
class RadianceEstimator;
class Sampler;
class Scene;

class EstimatorTileWork : public TileWork {
private:
    using Estimator = RadianceEstimator;

public:
    EstimatorTileWork(
        const Scene* const     scene,
        const Camera* const    camera,
        const Estimator* const estimator,
        const Sampler* const   sampler);

    void work() const override;

private:
    const Scene*     _scene;
    const Camera*    _camera;
    const Estimator* _estimator;
    const Sampler*   _sampler;
};

} // namespace cadise