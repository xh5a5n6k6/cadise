#pragma once

#include "core/renderer/renderer.h"

namespace cadise {

class VpmSetting;
class Sampler;

class VpmRenderer : public Renderer {
public:
    VpmRenderer(
        const std::shared_ptr<Sampler>&    sampler,
        const std::shared_ptr<VpmSetting>& setting);

    void render() const override;

private:
    std::shared_ptr<Sampler>    _sampler;
    std::shared_ptr<VpmSetting> _setting;
};

} // namespace cadise