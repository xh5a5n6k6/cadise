#pragma once

#include "core/renderer/photon-mapping/photon.h"

namespace cadise {

class PhotonCenterCalculator {
public:
    PhotonCenterCalculator() = default;

    Vector3R operator () (const Photon& photon) const {
        return photon.position();
    }
};

} // namespace cadise