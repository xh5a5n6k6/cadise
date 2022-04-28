#pragma once

namespace cadise
{

/*! @brief Base class which is noncopyable but movable.

Reference:
https://github.com/TzuChieh/Photon-v2/blob/sdl-1.0/Engine/Source/Utility/IMoveOnly.h

https://stackoverflow.com/questions/35743586/noncopyable-and-nonmovable-together
*/
class MovableOnly
{
protected:
    MovableOnly() = default;
    ~MovableOnly() = default;

    MovableOnly(const MovableOnly& other) = delete;
    MovableOnly& operator = (const MovableOnly& other) = delete;

    MovableOnly(MovableOnly&& other) = default;
    MovableOnly& operator = (MovableOnly&& other) = default;
};

} // namespace cadise