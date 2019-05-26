#pragma once

#include "math/type.h"

namespace cadise {

class Path;

namespace imageIO {

void save(Path path, int width, int height, uint8* data);

} // namespace imageIO

} // namespace cadise