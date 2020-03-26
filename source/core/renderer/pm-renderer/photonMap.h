#pragma once

#include "core/integral-tool/pointKdTree.h"
#include "core/renderer/pm-renderer/photonCenterCalculator.h"

namespace cadise {

using PhotonMap = PointKdTree<std::size_t, Photon, PhotonCenterCalculator>;

} // namespace cadise