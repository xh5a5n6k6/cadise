#pragma once

#include "Core/Renderer/Renderer.h"

#include "Core/Renderer/PhotonMapping/PMSetting.h"

// forward declaration
namespace cadise
{
    class Sampler;
}

namespace cadise
{

class VPMRenderer : public Renderer
{
public:
    VPMRenderer(
        const std::shared_ptr<Sampler>& sampler,
        const PMSetting&                setting);

    void render() const override;

private:
    std::shared_ptr<Sampler> _sampler;
    PMSetting                _setting;
};

} // namespace cadise