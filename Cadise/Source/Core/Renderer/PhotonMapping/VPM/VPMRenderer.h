#pragma once

#include "core/renderer/renderer.h"

#include "core/renderer/photon-mapping/pmSetting.h"

namespace cadise { class Sampler; }

namespace cadise 
{

class VpmRenderer : public Renderer 
{
public:
    VpmRenderer(
        const std::shared_ptr<Sampler>& sampler,
        const PmSetting&                setting);

    void render() const override;

private:
    std::shared_ptr<Sampler> _sampler;
    PmSetting                _setting;
};

} // namespace cadise