#include "FileIO/Path.h"

namespace cadise 
{

Path::Path(const std::string& path) :
    _path(path) 
{}

Path::Path(const std::string_view& path) :
    Path(std::string(path))
{}

bool Path::isExtendedWith(const std::string& extension) const 
{
    if (extension.length() > _path.length())
    {
        return false;
    }

    return _path.substr(_path.length() - extension.length()).compare(extension) == 0;
}

const std::string& Path::path() const
{
    return _path;
}

} // namespace cadise