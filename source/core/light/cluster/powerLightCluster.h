#pragma once

#include "core/light/cluster/lightCluster.h"

#include "math/distribution/distribution1D.h"

#include <memory>
#include <vector>

namespace cadise {

class PowerLightCluster : public LightCluster {
public:
    PowerLightCluster(const std::vector<std::shared_ptr<Light>>& lights);

    const Light* sampleOneLight(real* const out_pdf) const override;

private:
    std::vector<std::shared_ptr<Light>> _lights;
    Distribution1D _distribution;
};

} // namespace cadise