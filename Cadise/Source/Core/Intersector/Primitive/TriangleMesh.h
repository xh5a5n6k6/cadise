//#pragma once
//
//#include "core/intersector/primitive/primitive.h"
//#include "math/distribution/distribution1D.h"
//
//#include <memory>
//#include <vector>
//
//namespace cadise { class KdTreeAccelerator; }
//namespace cadise { class Triangle; }
//
//namespace cadise 
//{
//
//class TriangleMesh : public Primitive 
//{
//    TriangleMesh(
//        const std::vector<const Triangle*>& triangles,
//        std::unique_ptr<KdTreeAccelerator>  triangleKdTree);
//
//    void evaluateBound(AABB3R* const out_bound) const override;
//
//    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override;
//    bool isOccluded(const Ray& ray) const override;
//
//    void evaluateSurfaceDetail(
//        const PrimitiveInfo& primitiveInfo,
//        SurfaceDetail* const out_surface) const override;
//
//    void evaluatePositionSample(PositionSample* const out_sample) const override;
//    real evaluatePositionPdfA(const Vector3R& position) const override;
//    real area() const override;
//
//private:
//    std::vector<const Triangle*>       _triangles;
//    std::unique_ptr<KdTreeAccelerator> _triangleKdTree;
//
//    Distribution1D _areaDistribution;
//    real           _area;
//};
//
//} // namespace cadise