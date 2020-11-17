#pragma once

#include "core/renderer/renderer.h"

namespace cadise {

class Sampler;

class BdptRenderer : public Renderer {
public:
    explicit BdptRenderer(const std::shared_ptr<Sampler>& sampler);

    void render() const override;

private:
    std::shared_ptr<Sampler> _sampler;
};

} // namespace cadise