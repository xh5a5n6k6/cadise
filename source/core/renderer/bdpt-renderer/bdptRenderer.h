#pragma once

#include "core/renderer/renderer.h"

#include "math/type/fundamentalType.h"

namespace cadise {

class Sampler;

class BdptRenderer : public Renderer {
public:
    BdptRenderer(const std::shared_ptr<Sampler>& sampler);

    void render(const Scene& scene) const override;

private:
    std::shared_ptr<Sampler> _sampler;

    const static int32 MAX_PATH_DEPTH = 64;
};

} // namespace cadise