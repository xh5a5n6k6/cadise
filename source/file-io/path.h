#pragma once

#include <string>

namespace cadise {

class Path {
public:
    Path() = default;
    Path(std::string path);

    bool isExtendedWith(std::string extension);

    std::string path();

private:
    std::string _path;
};

} // namespace cadise