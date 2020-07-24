#pragma once

#include "core/renderer/vanilla-pm/photon.h"

namespace cadise {

class PhotonCenterCalculator {
public:
    PhotonCenterCalculator() = default;

    Vector3R operator () (const Photon& photon) const {
        return photon.position();
    }
};

} // namespace cadise