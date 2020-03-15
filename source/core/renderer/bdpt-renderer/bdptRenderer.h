#pragma once

#include "core/renderer/renderer.h"

#include "core/spectrum/spectrum.h"

namespace cadise {

class Ray;
class Sampler;

class BdptRenderer : public Renderer {
public:
    explicit BdptRenderer(const std::shared_ptr<Sampler>& sampler);

    void render(const Scene& scene) const override;

private:
    std::shared_ptr<Sampler> _sampler;

    const static std::size_t MAX_PATH_LENGTH = 32;
};

} // namespace cadise