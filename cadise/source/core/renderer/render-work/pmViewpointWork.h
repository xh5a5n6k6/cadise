#pragma once

#include "core/renderer/renderWork.h"

#include "math/tAabb2.h"

#include <vector>

namespace cadise {

class PmViewpoint;
class PmViewpointConstructor;
class Sampler;

class PmViewpointWork : public RenderWork {
public:
    PmViewpointWork(
        const AABB2I&                       sampleBound,
        const Sampler* const                sampler,
        const PmViewpointConstructor* const viewpointBuilder,
        std::vector<PmViewpoint>* const     viewpoints);

    void work() const override;

private:
    const Sampler*                _sampler;
    const PmViewpointConstructor* _viewpointConstructor;

    std::vector<PmViewpoint>* _viewpoints;

    AABB2I _sampleBound;
};

} // namespace cadise