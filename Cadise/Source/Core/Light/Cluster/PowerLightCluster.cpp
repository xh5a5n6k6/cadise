#include "Core/Light/Cluster/PowerLightCluster.h"

#include "Core/Light/Light.h"
#include "Foundation/Assertion.h"
#include "Math/Random.h"

namespace cadise
{

PowerLightCluster::PowerLightCluster(const std::vector<std::shared_ptr<Light>>& lights) :
    _lights(std::move(lights)),
    _lightIndexMap(),
    _distribution() 
{
    CS_ASSERT(!_lights.empty());

    const std::size_t numLights = _lights.size();

    std::vector<real> fluxVector(numLights);
    for (std::size_t i = 0; i < numLights; ++i) 
    {
        fluxVector[i] = _lights[i]->approximateFlux();

        // build reverse mapping from light pointer to index offsets
        _lightIndexMap[_lights[i].get()] = i;
    }

    _distribution = Distribution1D(fluxVector.data(), numLights);
}

const Light* PowerLightCluster::sampleOneLight(real* const out_pdf) const
{
    CS_ASSERT(out_pdf);

    const real        sample      = Random::nextReal();
    const std::size_t sampleIndex = _distribution.sampleDiscrete(sample, out_pdf);

    return _lights[sampleIndex].get();
}

real PowerLightCluster::evaluatePickLightPdf(const Light* const light) const 
{
    const auto&& iterator = _lightIndexMap.find(light);

    CS_ASSERT_NE(iterator, _lightIndexMap.end());

    const std::size_t lightIndex = iterator->second;

    return _distribution.pdfDiscrete(lightIndex);
}

} // namespace cadise