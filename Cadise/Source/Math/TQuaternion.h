#pragma once

#include "Math/TMatrix4.h"

namespace cadise 
{

template<typename T>
class TQuaternion 
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

#include "Math/TQuaternion.ipp"