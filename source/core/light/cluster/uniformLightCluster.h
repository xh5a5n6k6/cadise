#pragma once

#include "core/light/cluster/lightCluster.h"

#include <memory>
#include <vector>

namespace cadise {

class UniformLightCluster : public LightCluster {
public:
    UniformLightCluster(const std::vector<std::shared_ptr<Light>>& lights);

    const Light* sampleOneLight(real* const out_pdf) const override;
    real evaluatePickLightPdf(const Light* const light) const override;

private:
    std::vector<std::shared_ptr<Light>> _lights;
};

} // namespace cadise