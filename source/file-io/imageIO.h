#pragma once

#include "math/type.h"

namespace cadise {

class Path;

namespace imageIO {

void save(Path path, int32 width, int32 height, uint8* data);

} // namespace imageIO

} // namespace cadise