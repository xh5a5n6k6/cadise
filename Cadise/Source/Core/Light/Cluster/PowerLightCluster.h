#pragma once

#include "core/light/cluster/lightCluster.h"

#include "math/distribution/distribution1D.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace cadise 
{

class PowerLightCluster : public LightCluster
{
public:
    explicit PowerLightCluster(const std::vector<std::shared_ptr<Light>>& lights);

    const Light* sampleOneLight(real* const out_pdf) const override;
    real evaluatePickLightPdf(const Light* const light) const override;

private:
    std::vector<std::shared_ptr<Light>>           _lights;
    std::unordered_map<const Light*, std::size_t> _lightIndexMap;

    Distribution1D _distribution;
};

} // namespace cadise