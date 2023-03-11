#pragma once

#include "Core/Renderer/Renderer.h"

// forward declaration
namespace cadise
{
    class Sampler;
}

namespace cadise 
{

class BDPTRenderer : public Renderer 
{
public:
    explicit BDPTRenderer(const std::shared_ptr<Sampler>& sampler);

    void render() const override;

private:
    std::shared_ptr<Sampler> _sampler;
};

} // namespace cadise