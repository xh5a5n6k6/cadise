#pragma once

#include "Core/Gear/TPointKDTree.h"
#include "Core/Renderer/PhotonMapping/PhotonCenterCalculator.h"

namespace cadise
{

using PhotonMap = TPointKDTree<std::size_t, Photon, PhotonCenterCalculator>;

} // namespace cadise