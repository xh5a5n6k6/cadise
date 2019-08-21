#pragma once

#include "core/intersection.h"

#include "core/intersector/primitive/primitiveInfo.h"
#include "core/surfaceInfo.h"

namespace cadise {

class SurfaceIntersection : public Intersection {
public:
    SurfaceIntersection();
    SurfaceIntersection(const SurfaceIntersection& surfaceIntersection);

    PrimitiveInfo primitiveInfo() const;
    SurfaceInfo surfaceInfo() const;
    Vector3R wi() const;
    Vector3R wo() const;
    real pdf() const;

    void setPrimitiveInfo(const PrimitiveInfo& primitiveInfo);
    void setSurfaceInfo(const SurfaceInfo& surfaceInfo);
    void setWi(const Vector3R& wi);
    void setWo(const Vector3R& wo);
    void setPdf(const real pdf);

private:
    PrimitiveInfo _primitiveInfo;
    SurfaceInfo _surfaceInfo;
    Vector3R _wi;
    Vector3R _wo;

    real _pdf;
};

} // namespace cadise