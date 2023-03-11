#pragma once

#include "Core/Renderer/Renderer.h"

// forward declaration
namespace cadise
{
    class EnergyEstimator;
    class Sampler;
}

namespace cadise
{

class SamplingRenderer : public Renderer
{
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