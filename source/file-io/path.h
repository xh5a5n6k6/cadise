#pragma once

#include <string>

namespace cadise {

class Path {
public:
    Path(const std::string& path);

    bool isExtendedWith(const std::string& extension) const;

    std::string path() const;

private:
    std::string _path;
};

} // namespace cadise