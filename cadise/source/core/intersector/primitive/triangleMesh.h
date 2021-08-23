//#pragma once
//
//#include "core/intersector/primitive/primitive.h"
//
//#include <memory>
//
//namespace cadise {
//
//class KdTreeAccelerator;
//
//class TriangleMesh : public Primitive {
//    TriangleMesh(
//        const std::shared_ptr<Bsdf>& bsdf,
//        const Vector3R& vA,
//        const Vector3R& vB,
//        const Vector3R& vC);
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
//    std::vector<std::unique_ptr<Triangle>> _triangles;
//
//    std::unique_ptr<KdTreeAccelerator> _triangleKdTree;
//};
//
//} // namespace cadise