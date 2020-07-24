#pragma once

#include "core/renderer/renderer.h"

namespace cadise {

class PmSetting;
class Sampler;

class PmRenderer : public Renderer {
public:
    PmRenderer(
        const std::shared_ptr<Sampler>&   sampler,
        const std::shared_ptr<PmSetting>& setting);

    void render() const override;

private:
    std::shared_ptr<Sampler>   _sampler;
    std::shared_ptr<PmSetting> _setting;
};

} // namespace cadise