#pragma once

#include "core/renderer/pm-renderer/photon.h"

namespace cadise {

class PhotonCenterCalculator {
public:
    PhotonCenterCalculator() = default;

    Vector3R operator () (const Photon& photon) const {
        return photon.position();
    }
};

} // namespace cadise