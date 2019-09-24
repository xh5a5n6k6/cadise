#pragma once

#include "core/spectrum/spectrum.h"

#include "file-io/path.h"

#include <memory>

namespace cadise {

class Filter;
class Pixel;

class Film {
public:
    Film(const int32 widthPx, const int32 heightPx,
         const Path& filename,
         const std::shared_ptr<Filter>& filter);

    void addSample(const Vector2R& filmPosition, const Spectrum& value);
    void save() const;

    Vector2I resolution() const;

private:
    Vector2I                _resolution;
    Path                    _filename;
    std::shared_ptr<Filter> _filter;

    Pixel* _pixels;
};

} // namespace cadise