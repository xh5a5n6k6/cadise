#pragma once

#include "core/intersection.h"

#include "core/intersector/primitiveInfo.h"
#include "core/surfaceDetail.h"

namespace cadise {

class SurfaceIntersection : public Intersection {
public:
    SurfaceIntersection();

    // return wi/wo swap SurfaceIntersection
    SurfaceIntersection reverse() const;

    const PrimitiveInfo& primitiveInfo() const;
    const SurfaceDetail& surfaceDetail() const;
    const Vector3R& wi() const;
    const Vector3R& wo() const;

    void setPrimitiveInfo(const PrimitiveInfo& primitiveInfo);
    void setSurfaceDetail(const SurfaceDetail& surfaceDetail);
    void setWi(const Vector3R& wi);
    void setWo(const Vector3R& wo);

private:
    PrimitiveInfo _primitiveInfo;
    SurfaceDetail _surfaceDetail;
    Vector3R      _wi;
    Vector3R      _wo;
};

} // namespace cadise