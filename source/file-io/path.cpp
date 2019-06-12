#include "file-io/path.h"

namespace cadise {

Path::Path(const std::string path) :
    _path(path) {
}

bool Path::isExtendedWith(const std::string extension) const {
    if (extension.length() > _path.length()) {
        return false;
    }

    return _path.substr(_path.length() - extension.length()).compare(extension) == 0;
}

std::string Path::path() const {
    return _path;
}

} // namespace cadise