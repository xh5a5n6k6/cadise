#include "Core/Intersector/Primitive/TriangleBuffer.h"

#include "Core/Intersector/Primitive/Triangle.h"
#include "Foundation/Assertion.h"

namespace cadise
{

TriangleBuffer::TriangleBuffer(
    const std::shared_ptr<BSDF>& bsdf,
    const std::vector<Vector3R>& positions,
    const std::vector<Vector3R>& normals,
    const std::vector<Vector3R>& uvws) :

    _bsdf(bsdf),
    _positions(positions),
    _normals(normals),
    _uvws(uvws)
{
    CS_ASSERT(bsdf);
}

void TriangleBuffer::transformToTriangles(
    std::vector<std::shared_ptr<Primitive>>* const out_triangles) const
{
    CS_ASSERT(out_triangles);

    for (std::size_t i = 0; i < _positions.size(); i += 3)
    {
        std::shared_ptr<Triangle> triangle
            = std::make_shared<Triangle>(_bsdf, _positions[i], _positions[i + 1], _positions[i + 2]);

        if (!_normals.empty())
        {
            triangle->setNa(_normals[i + 0]);
            triangle->setNb(_normals[i + 1]);
            triangle->setNc(_normals[i + 2]);
        }
        if (!_uvws.empty())
        {
            triangle->setUvwA(_uvws[i + 0]);
            triangle->setUvwB(_uvws[i + 1]);
            triangle->setUvwC(_uvws[i + 2]);
        }

        out_triangles->push_back(std::move(triangle));
    }
}

} // namespace cadise