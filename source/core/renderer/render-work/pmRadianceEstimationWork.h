#pragma once

#include "core/renderer/renderWork.h"

#include "core/renderer/photon-mapping/photonMap.h"

#include <vector>

namespace cadise {

class Film;
class PmViewpoint;
class Scene;

class PmRadianceEstimationWork : public RenderWork {
public:
    PmRadianceEstimationWork(
        const PhotonMap* const    photonMap,
        std::vector<PmViewpoint>* viewpoints,
        Film*                     film,
        const std::size_t         numPhotonPaths,
        const real                alpha);

    void work() const override;

    void setWorkBeginEnd(
        const std::size_t beginIndex,
        const std::size_t endIndex);

private:
    const PhotonMap* _photonMap;

    std::vector<PmViewpoint>* _viewpoints;
    Film*                     _film;

    std::size_t _numPhotonPaths;
    real        _alpha;

    std::size_t _workBeginIndex;
    std::size_t _workEndIndex;
};

} // namespace cadise