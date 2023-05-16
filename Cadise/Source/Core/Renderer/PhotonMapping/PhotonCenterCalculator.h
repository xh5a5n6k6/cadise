#pragma once

#include "Core/Renderer/PhotonMapping/Photon.h"

namespace cadise
{

class PhotonCenterCalculator
{
public:
    PhotonCenterCalculator() = default;

    Vector3R operator () (const Photon& photon) const
    {
        return photon.position();
    }
};

} // namespace cadise