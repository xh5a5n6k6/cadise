#pragma once

namespace cadise {

class Ray;

class Camera {
public:
    virtual Ray createRay(int px, int py) = 0;
};

} // namespace cadise