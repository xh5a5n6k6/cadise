#pragma once

#include "math/type/fundamentalType.h"

#include <vector>

namespace cadise {

class Photon;
class Scene;

class PmProcess {
public:
    PmProcess(const Scene* const scene);

    void process(
        std::vector<Photon>* const out_photons,
        std::size_t* const         out_numPhotonPaths) const;

    void setMaxNumPhotons(const std::size_t maxNumPhotons);
    void setMaxNumPhotonPaths(const std::size_t maxNumPhotonPaths);

private:
    const Scene* _scene;

    std::size_t _maxNumPhotons;
    std::size_t _maxNumPhotonPaths;
};

} // namespace cadise