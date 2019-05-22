#include "file-io/path.h"

namespace cadise {

Path::Path() :
    _path("") {
}

Path::Path(std::string path) :
    _path(path) {
}

bool Path::isExtendedWith(std::string extension) {
    if (extension.length() > _path.length()) {
        return false;
    }

    return _path.substr(_path.length() - 4).compare(extension) == 0;
}

std::string Path::path() {
    return _path;
}

} // namespace cadise