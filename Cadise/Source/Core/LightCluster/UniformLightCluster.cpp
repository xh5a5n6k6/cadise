#include "Core/LightCluster/UniformLightCluster.h"

#include "Core/Light/Light.h"
#include "Foundation/Assertion.h"
#include "Math/Random.h"

namespace cadise
{

UniformLightCluster::UniformLightCluster(const std::vector<std::shared_ptr<Light>>& lights) :
    LightCluster(),
    _lights(std::move(lights))
{
    CS_ASSERT(!_lights.empty());
}

const Light* UniformLightCluster::sampleOneLight(real* const out_pdf) const
{
    CS_ASSERT(out_pdf);

    const std::size_t numLights   = _lights.size();
    const std::size_t sampleIndex = Random::nextIndex(0, numLights);

    *out_pdf = 1.0_r / static_cast<real>(numLights);

    return _lights[sampleIndex].get();
}

real UniformLightCluster::evaluatePickLightPdf(const Light* const light) const
{
    CS_ASSERT(light);

    return 1.0_r / static_cast<real>(_lights.size());
}

} // namespace cadise