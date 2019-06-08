#include "file-io/path.h"

namespace cadise {

Path::Path(std::string path) :
    _path(path) {
}

bool Path::isExtendedWith(std::string extension) {
    if (extension.length() > _path.length()) {
        return false;
    }

    return _path.substr(_path.length() - extension.length()).compare(extension) == 0;
}

std::string Path::path() {
    return _path;
}

} // namespace cadise