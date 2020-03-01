#include "core/light/cluster/powerLightCluster.h"

#include "core/light/light.h"
#include "fundamental/assertion.h"
#include "math/random.h"

namespace cadise {

PowerLightCluster::PowerLightCluster(const std::vector<std::shared_ptr<Light>>& lights) :
    _lights(std::move(lights)),
    _distribution() {

    CADISE_ASSERT(!_lights.empty());

    const std::size_t numLights = _lights.size();

    std::vector<real> fluxVector(numLights);
    for (std::size_t i = 0; i < numLights; ++i) {
        fluxVector[i] = _lights[i]->approximatedFlux();
    }

    _distribution = Distribution1D(fluxVector.data(), numLights);
}

const Light* PowerLightCluster::sampleOneLight(real* const out_pdf) const {
    CADISE_ASSERT(out_pdf);

    const real sample = Random::nextReal();
    const std::size_t sampleIndex = _distribution.sampleDiscrete(sample, out_pdf);

    return _lights[sampleIndex].get();
}

} // namespace cadise