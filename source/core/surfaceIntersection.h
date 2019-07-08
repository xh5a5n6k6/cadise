#pragma once

#include "core/intersection.h"

#include "core/primitiveInfo.h"
#include "core/surfaceGeometryInfo.h"
#include "core/surfaceShadingInfo.h"

namespace cadise {

class SurfaceIntersection : public Intersection {
public:
    SurfaceIntersection();

    PrimitiveInfo primitiveInfo() const;
    SurfaceGeometryInfo surfaceGeometryInfo() const;
    SurfaceShadingInfo surfaceShadingInfo() const;
    Vector3R wi() const;
    Vector3R wo() const;
    real pdf() const;

    void setPrimitiveInfo(const PrimitiveInfo& primitiveInfo);
    void setSurfaceGeometryInfo(const SurfaceGeometryInfo& geometryInfo);
    void setSurfaceShadingInfo(const SurfaceShadingInfo& shadingInfo);
    void setWi(const Vector3R& wi);
    void setWo(const Vector3R& wo);
    void setPdf(const real pdf);

private:
    PrimitiveInfo _primitiveInfo;
    SurfaceGeometryInfo _geometryInfo;
    SurfaceShadingInfo _shadingInfo;
    Vector3R _wi;
    Vector3R _wo;

    real _pdf;
};

} // namespace cadise