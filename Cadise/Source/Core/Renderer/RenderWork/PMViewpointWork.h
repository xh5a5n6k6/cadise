#pragma once

#include "Core/Renderer/RenderWork.h"

#include "Math/TAABB2.h"

#include <vector>

// forward declaration
namespace cadise
{
    class PMViewpoint;
    class PMViewpointConstructor;
    class Sampler;
}

namespace cadise
{

class PMViewpointWork : public RenderWork
{
public:
    PMViewpointWork(
        const AABB2I&                       sampleBound,
        const Sampler* const                sampler,
        const PMViewpointConstructor* const viewpointBuilder,
        std::vector<PMViewpoint>* const     viewpoints);

    void work() const override;

private:
    const Sampler*                _sampler;
    const PMViewpointConstructor* _viewpointConstructor;

    std::vector<PMViewpoint>* _viewpoints;

    AABB2I _sampleBound;
};

} // namespace cadise