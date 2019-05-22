#pragma once

namespace cadise {

class Film;
class Ray;

class Camera {
public:
    virtual Ray createRay(int px, int py) = 0;

    virtual Film film() = 0;
};

} // namespace cadise