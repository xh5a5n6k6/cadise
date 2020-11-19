#pragma once

#include "core/renderer/renderer.h"

namespace cadise {

class RadianceEstimator;
class Sampler;

class SamplingRenderer : public Renderer {
private:
    using Estimator = RadianceEstimator;

public:
    SamplingRenderer(
        const std::shared_ptr<Estimator>& estimator,
        const std::shared_ptr<Sampler>&   sampler);

    void render() const override;

private:
    std::shared_ptr<Estimator> _estimator;
    std::shared_ptr<Sampler>   _sampler;
};

} // namespace cadise