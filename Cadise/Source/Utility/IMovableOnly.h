#pragma once

namespace cadise
{

/*! @brief Base class which is non-copyable but movable.

Reference:
https://github.com/TzuChieh/Photon-v2/blob/sdl-1.0/Engine/Source/Utility/IMoveOnly.h

https://stackoverflow.com/questions/35743586/noncopyable-and-nonmovable-together
*/
class IMovableOnly
{
protected:
    IMovableOnly() = default;
    ~IMovableOnly() = default;

    IMovableOnly(const IMovableOnly& other) = delete;
    IMovableOnly& operator = (const IMovableOnly& other) = delete;

    IMovableOnly(IMovableOnly&& other) = default;
    IMovableOnly& operator = (IMovableOnly&& other) = default;
};

} // namespace cadise