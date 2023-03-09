#pragma once

#include "core/integral-tool/tPointKdTree.h"
#include "core/renderer/photon-mapping/photonCenterCalculator.h"

namespace cadise 
{

using PhotonMap = TPointKdTree<std::size_t, Photon, PhotonCenterCalculator>;

} // namespace cadise