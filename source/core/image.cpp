#include "core/image.h"

namespace cadise {

Image::Image() :
    Image(0, 0) {
}

Image::Image(int rx, int ry) :
    _resolutionX(rx), _resolutionY(ry) {
}

int Image::resolutionX() {
    return _resolutionX;
}

int Image::resolutionY() {
    return _resolutionY;
}

} // namespace cadise