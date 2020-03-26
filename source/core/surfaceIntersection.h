#pragma once

#include "core/intersection.h"

#include "core/intersector/primitiveInfo.h"
#include "core/surfaceInfo.h"

namespace cadise {

class SurfaceIntersection : public Intersection {
public:
    SurfaceIntersection();

    // return wi/wo swap SurfaceIntersection
    SurfaceIntersection reverse() const;

    const PrimitiveInfo& primitiveInfo() const;
    const SurfaceInfo& surfaceInfo() const;
    const Vector3R& wi() const;
    const Vector3R& wo() const;

    void setPrimitiveInfo(const PrimitiveInfo& primitiveInfo);
    void setSurfaceInfo(const SurfaceInfo& surfaceInfo);
    void setWi(const Vector3R& wi);
    void setWo(const Vector3R& wo);

private:
    PrimitiveInfo _primitiveInfo;
    SurfaceInfo   _surfaceInfo;
    Vector3R      _wi;
    Vector3R      _wo;
};

} // namespace cadise