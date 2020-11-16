#pragma once

#include "core/renderer/renderer.h"

namespace cadise {

class Integrator;
class Sampler;

class SamplingRenderer : public Renderer {
public:
    SamplingRenderer(
        const std::shared_ptr<Integrator>& integrator,
        const std::shared_ptr<Sampler>&    sampler);

    void render() const override;

private:
    std::shared_ptr<Integrator> _integrator;
    std::shared_ptr<Sampler>    _sampler;
};

} // namespace cadise