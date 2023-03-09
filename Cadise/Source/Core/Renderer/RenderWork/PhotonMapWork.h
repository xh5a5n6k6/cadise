#pragma once

#include "core/renderer/renderWork.h"

#include <vector>

namespace cadise { class Photon; }
namespace cadise { class Scene; }

namespace cadise
{

class PhotonMapWork : public RenderWork
{
public:
    PhotonMapWork(
        const Scene* const         scene,
        std::vector<Photon>* const photons,
        std::size_t* const         numPhotonPaths);

    void work() const;

    void setMaxNumPhotons(const std::size_t maxNumPhotons);
    void setMaxNumPhotonPaths(const std::size_t maxNumPhotonPaths);

private:
    const Scene*         _scene;
    std::vector<Photon>* _photons;
    std::size_t*         _numPhotonPaths;

    std::size_t _maxNumPhotons;
    std::size_t _maxNumPhotonPaths;
};

} // namespace cadise