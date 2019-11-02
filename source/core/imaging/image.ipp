#pragma once

#include "core/imaging/image.h"

#include "math/vector.h"

namespace cadise {

template<typename T, std::size_t N>
inline Image<T, N>::Image() :
    Image(0, 0) {
}

template<typename T, std::size_t N>
inline Image<T, N>::Image(const Vector2I& resolution) :
    Image(resolution.x(), resolution.y()) {
}

template<typename T, std::size_t N>
inline Image<T, N>::Image(const int32 width, const int32 height) :
    _width(width),
    _height(height),
    _data() {

    const std::size_t dataNumber = static_cast<std::size_t>(width * height * N);
    setDataSize(dataNumber);
}

template<typename T, std::size_t N>
inline void Image<T, N>::setImageSize(const Vector2I& resolution) {
    setImageSize(resolution.x(), resolution.y());
}

template<typename T, std::size_t N>
inline void Image<T, N>::setImageSize(const int32 width, const int32 height) {
    _width  = width;
    _height = height;

    const std::size_t dataNumber = static_cast<std::size_t>(width * height * N);
    setDataSize(dataNumber);
}

template<typename T, std::size_t N>
inline void Image<T, N>::setDataSize(const std::size_t dataSize) {
    _data.resize(dataSize);
}

template<typename T, std::size_t N>
inline void Image<T, N>::setPixelValue(const int32 x, const int32 y, const Vector<T, N>& pixelValue) {
    const std::size_t dataIndexOffset = _pixelDataOffset(x, y);

    if constexpr (N == 3) {
        _data[dataIndexOffset + 0] = pixelValue[0];
        _data[dataIndexOffset + 1] = pixelValue[1];
        _data[dataIndexOffset + 2] = pixelValue[2];
    }
    else if constexpr (N == 4) {
        _data[dataIndexOffset + 0] = pixelValue[0];
        _data[dataIndexOffset + 1] = pixelValue[1];
        _data[dataIndexOffset + 2] = pixelValue[2];
        _data[dataIndexOffset + 3] = pixelValue[3];
    }
    else {
        // something wrong
    }
}

template<typename T, std::size_t N>
inline void Image<T, N>::setDataValue(const std::size_t index, const T value) {
    _data[index] = value;
}

template<typename T, std::size_t N>
inline void Image<T, N>::getImagePixel(const int32 x, const int32 y, Vector<T, N>* const out_pixel) const {
    const std::size_t dataIndexOffset = _pixelDataOffset(x, y);
    
    if constexpr (N == 3) {
        *out_pixel = Vector<T, N>(_data[dataIndexOffset + 0],
                                  _data[dataIndexOffset + 1],
                                  _data[dataIndexOffset + 2]);
    }
    else if constexpr (N == 4) {
        *out_pixel = Vector<T, N>(_data[dataIndexOffset + 0],
                                  _data[dataIndexOffset + 1],
                                  _data[dataIndexOffset + 2],
                                  _data[dataIndexOffset + 3]);
    }
    else {
        // something wrong
    }
}

template<typename T, std::size_t N>
inline int32 Image<T, N>::width() const {
    return _width;
}

template<typename T, std::size_t N>
inline int32 Image<T, N>::height() const {
    return _height;
}

template<typename T, std::size_t N>
inline std::size_t Image<T, N>::dataSize() const {
    return _data.size();
}

template<typename T, std::size_t N>
inline const T* Image<T, N>::rawData() const {
    return _data.data();
}

template<typename T, std::size_t N>
inline std::size_t Image<T, N>::_pixelDataOffset(const int32 x, const int32 y) const {
    return static_cast<std::size_t>(x + y * _width) * N;
}

} // namespace cadise