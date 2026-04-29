//#include "Core/intersector/primitive/triangleMesh.h"
//
//#include "Core/integral-tool/sample/positionSample.h"
//#include "Core/intersector/accelerator/kd-tree/kdTreeAccelerator.h"
//#include "Core/intersector/primitive/triangle.h"
//#include "fundamental/assertion.h"
//#include "math/random.h"
//
//namespace cadise
//{
//
//TriangleMesh::TriangleMesh(
//    const std::vector<const Triangle*>& triangles,
//    std::unique_ptr<KDTreeAccelerator>  triangleKdTree) :
//
//    Primitive(),
//    _triangles(triangles),
//    _triangleKdTree(std::move(triangleKdTree)),
//    _area(0.0_r)
//{
//    std::vector<real> areas(triangles.size());
//    for (std::size_t i = 0; i < areas.size(); ++i)
//    {
//        areas[i]  = _triangles[i]->area();
//        _area    += areas[i];
//    }
//
//    _areaDistribution = Distribution1D(areas.data(), areas.size());
//}
//
//void TriangleMesh::evaluateBound(AABB3R* const out_bound) const
//{
//    CS_ASSERT(out_bound);
//
//    _triangleKdTree->evaluateBound(out_bound);
//}
//
//bool TriangleMesh::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const
//{
//    return _triangleKdTree->isIntersecting(ray, primitiveInfo);
//}
//
//bool TriangleMesh::isOccluded(const Ray& ray) const
//{
//    return _triangleKdTree->isOccluded(ray);
//}
//
//void TriangleMesh::evaluateSurfaceDetail(
//    const PrimitiveInfo& primitiveInfo,
//    SurfaceDetail* const out_surface) const
//{
//    // it should never be called in this class
//    CS_ASSERT(false);
//}
//
//void TriangleMesh::evaluatePositionSample(PositionSample* const out_sample) const
//{
//    CS_ASSERT(out_sample);
//
//    const real        sample      = Random::nextReal();
//    const std::size_t sampleIndex = _areaDistribution.sampleDiscrete(sample);
//
//    _triangles[sampleIndex]->evaluatePositionSample(out_sample);
//    if (!out_sample->isValid())
//    {
//        return;
//    }
//
//    out_sample->setPdfA(1.0_r / _area);
//}
//
//real TriangleMesh::evaluatePositionPdfA(const Vector3R& position) const
//{
//    return 1.0_r / _area;
//}
//
//real TriangleMesh::area() const
//{
//    return _area;
//}
//
//} // namespace cadise