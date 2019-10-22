#pragma once

#include "math/type/fundamentalType.h"

namespace cadise {

class Path;

namespace imageIO {

void save(const Path& path, const int32 width, const int32 height, const uint8* data);

} // namespace imageIO

} // namespace cadise