#pragma once

#include "core/basicHeader.h"

namespace cadise {

class Image {
public:
    Image();
    Image(int rx, int ry);

    void writeInImage(std::string filename){}

    int resolutionX();
    int resolutionY();

    int _resolutionX, _resolutionY;
};

} // namespace cadise