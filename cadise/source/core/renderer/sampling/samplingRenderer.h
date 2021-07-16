#pragma once

#include "core/renderer/renderer.h"

namespace cadise {

class EnergyEstimator;
class Sampler;

class SamplingRenderer : public Renderer {
public:
    SamplingRenderer(
        const std::shared_ptr<EnergyEstimator>& estimator,
        const std::shared_ptr<Sampler>&         sampler);

    void render() const override;

private:
    std::shared_ptr<EnergyEstimator> _estimator;
    std::shared_ptr<Sampler>         _sampler;
};

} // namespace cadise