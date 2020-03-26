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

    //// number of photons stored in photon map per iteration
    //std::size_t _numPhotons;

    //std::size_t _numIterations;
    //std::size_t _numSpp;
};

} // namespace cadise