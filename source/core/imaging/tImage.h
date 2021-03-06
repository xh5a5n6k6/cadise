#pragma once

#include "math/type/mathType.h"

#include <vector>

namespace cadise {

/*
    TImage represents value array.

    T: value type. 
        [real]  for hdr image, 
        [uint8] for ldr image.
    N: value number per pixel. 
        [3] for normal image (rgb), 
        [4] for alpha image (rgba).
*/
template<typename T, std::size_t N>
class TImage {
public:
    TImage();
    explicit TImage(const Vector2I& resolution);
    TImage(const int32 width, const int32 height);
    TImage(const TImage& rhs);

    void flipHorizontal();

    void setImageSize(const Vector2I& resolution);
    void setImageSize(const int32 width, const int32 height);
    void setDataSize(const std::size_t dataSize);

    void setPixelValue(const int32 x, const int32 y, const TVector<T, N>& pixelValue);
    void setDataValue(const std::size_t index, const T value);

    void getImagePixel(const int32 x, const int32 y, TVector<T, N>* const out_pixel) const;

    int32 width() const;
    int32 height() const;
    Vector2S resolution() const;
    std::size_t dataSize() const;
    const T* rawData() const;

private:
    std::size_t _pixelDataOffset(const int32 x, const int32 y) const;

    int32          _width;
    int32          _height;
    std::vector<T> _data;
};

} // namespace cadise

#include "core/imaging/tImage.ipp"