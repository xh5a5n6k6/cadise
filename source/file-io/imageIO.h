#pragma once

#include "file-io/path.h"

#include "math/type.h"

namespace cadise {

namespace imageIO {

void save(Path path, int width, int height, uint8* data);

} // namespace imageIO

} // namespace cadise