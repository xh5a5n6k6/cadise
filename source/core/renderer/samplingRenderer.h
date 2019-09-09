#pragma once

#include "core/renderer/renderer.h"

#include "math/fundamentalType.h"

#include <memory>

namespace cadise {

class Integrator;
class Sampler;

class SamplingRenderer : public Renderer {
public:
    SamplingRenderer(const std::shared_ptr<Integrator>& integrator, const int32 sampleNumber);

    void render(const Scene& scene) const override;

private:
    std::shared_ptr<Integrator> _integrator;
    //std::shared_ptr<Sampler> _sampler;

    int32 _sampleNumber;
};

} // namespace cadise