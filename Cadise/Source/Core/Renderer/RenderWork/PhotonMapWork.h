#pragma once

#include "Core/Renderer/RenderWork.h"

#include <vector>

// forward declaration
namespace cadise
{
    class Photon;
    class Scene;
}

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