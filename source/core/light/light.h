#pragma once

namespace cadise {

class Vector3;

class Light {
public:
    virtual Vector3 position() = 0;
    virtual Vector3 color() = 0;
};

} // namespace cadise