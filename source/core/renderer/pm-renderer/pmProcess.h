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
        const std::size_t          maxNumPhotons,
        std::vector<Photon>* const out_photons,
        std::size_t* const         out_numPhotonPaths) const;

private:
    const Scene* _scene;
};

} // namespace cadise