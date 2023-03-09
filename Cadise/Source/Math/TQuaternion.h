#pragma once

#include "math/tMatrix4.h"

namespace cadise 
{

template<typename T>
class Quaternion 
{
public:

    //Matrix4 toRotationMatrix() const;

private:
    T w;
    T x;
    T y;
    T z;
};

} // namespace cadise