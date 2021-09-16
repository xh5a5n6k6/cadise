#pragma once

#include <string>
#include <string_view>

namespace cadise 
{

class Path 
{
public:
    explicit Path(const std::string& path);
    explicit Path(const std::string_view& path);

    bool isExtendedWith(const std::string& extension) const;

    const std::string& path() const;

private:
    std::string _path;
};

} // namespace cadise