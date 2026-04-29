#pragma once

#include "Core/Renderer/RenderWork.h"

#include "Core/Renderer/PhotonMapping/PhotonMap.h"

#include <vector>

// forward declaration
namespace cadise
{
    class Film;
    class PMViewpoint;
    class Scene;
}

namespace cadise
{

class PMRadianceEstimationWork : public RenderWork
{
public:
    PMRadianceEstimationWork(
        const PhotonMap* const    photonMap,
        std::vector<PMViewpoint>* viewpoints,
        Film*                     film,
        const std::size_t         numPhotonPaths,
        const real                alpha);

    void work() const override;

    void setWorkBeginEnd(
        const std::size_t beginIndex,
        const std::size_t endIndex);

private:
    const PhotonMap* _photonMap;

    std::vector<PMViewpoint>* _viewpoints;
    Film*                     _film;

    std::size_t _numPhotonPaths;
    real        _alpha;

    std::size_t _workBeginIndex;
    std::size_t _workEndIndex;
};

} // namespace cadise