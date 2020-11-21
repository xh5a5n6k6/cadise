#pragma once

#include "core/imaging/tImage.h"

#include "fundamental/assertion.h"
#include "math/math.h"
#include "math/tVector.h"

namespace cadise {

template<typename T, std::size_t N>
inline TImage<T, N>::TImage() :
    TImage(0, 0) {
}

template<typename T, std::size_t N>
inline TImage<T, N>::TImage(const Vector2I& resolution) :
    TImage(resolution.x(), resolution.y()) {
}

template<typename T, std::size_t N>
inline TImage<T, N>::TImage(const int32 width, const int32 height) :
    _width(width),
    _height(height),
    _data() {

    const std::size_t dataNumber = static_cast<std::size_t>(width * height * N);
    setDataSize(dataNumber);
}

template<typename T, std::size_t N>
inline TImage<T, N>::TImage(const TImage<T, N>& rhs) = default;

template<typename T, std::size_t N>
inline void TImage<T, N>::flipHorizontal() {
    const int32 halfWidth = _width / 2;

    for (int32 iy = 0; iy < _height; ++iy) {
        for (int32 ix = 0; ix < halfWidth; ++ix) {
            const std::size_t leftIndex  = _pixelDataOffset(ix, iy);
            const std::size_t rightIndex = _pixelDataOffset(_width - 1 - ix, iy);

            for (std::size_t i = 0; i < N; ++i) {
                math::swap(_data[leftIndex + i], _data[rightIndex + i]);
            }
        }
    }
}

template<typename T, std::size_t N>
inline void TImage<T, N>::setImageSize(const Vector2I& resolution) {
    this->setImageSize(resolution.x(), resolution.y());
}

template<typename T, std::size_t N>
inline void TImage<T, N>::setImageSize(const int32 width, const int32 height) {
    _width  = width;
    _height = height;

    const std::size_t dataNumber = static_cast<std::size_t>(width * height * N);
    this->setDataSize(dataNumber);
}

template<typename T, std::size_t N>
inline void TImage<T, N>::setDataSize(const std::size_t dataSize) {
    _data.resize(dataSize);
}

template<typename T, std::size_t N>
inline void TImage<T, N>::setPixelValue(const int32 x, const int32 y, const TVector<T, N>& pixelValue) {
    const std::size_t dataIndexOffset = _pixelDataOffset(x, y);

    for (std::size_t i = 0; i < N; ++i) {
        _data[dataIndexOffset + i] = pixelValue[i];
    }
}

template<typename T, std::size_t N>
inline void TImage<T, N>::setDataValue(const std::size_t index, const T value) {
    _data[index] = value;
}

template<typename T, std::size_t N>
inline void TImage<T, N>::getImagePixel(const int32 x, const int32 y, TVector<T, N>* const out_pixel) const {
    CADISE_ASSERT(out_pixel);
    CADISE_ASSERT_RANGE_INCLUSIVE(x, 0, _width - 1);
    CADISE_ASSERT_RANGE_INCLUSIVE(y, 0, _height - 1);

    const std::size_t dataIndexOffset = _pixelDataOffset(x, y);

    for (std::size_t i = 0; i < N; ++i) {
        (*out_pixel)[i] = _data[dataIndexOffset + i];
    }
}

template<typename T, std::size_t N>
inline int32 TImage<T, N>::width() const {
    return _width;
}

template<typename T, std::size_t N>
inline int32 TImage<T, N>::height() const {
    return _height;
}

template<typename T, std::size_t N>
inline Vector2S TImage<T, N>::resolution() const {
    return Vector2S(_width, _height);
}

template<typename T, std::size_t N>
inline std::size_t TImage<T, N>::dataSize() const {
    return _data.size();
}

template<typename T, std::size_t N>
inline const T* TImage<T, N>::rawData() const {
    return _data.data();
}

template<typename T, std::size_t N>
inline std::size_t TImage<T, N>::_pixelDataOffset(const int32 x, const int32 y) const {
    return static_cast<std::size_t>(x + y * _width) * N;
}

} // namespace cadise