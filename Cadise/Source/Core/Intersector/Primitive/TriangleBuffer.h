#pragma once

#include "Math/TVector3.h"

#include <memory>
#include <vector>

// forward declaration
namespace cadise
{
    class BSDF;
    class Primitive;
}

namespace cadise
{

/*

*/
class TriangleBuffer
{
public:
    TriangleBuffer(
        const std::shared_ptr<BSDF>& bsdf,
        const std::vector<Vector3R>& positions,
        const std::vector<Vector3R>& normals,
        const std::vector<Vector3R>& uvws);

    void transformToTriangles(
        std::vector<std::shared_ptr<Primitive>>* const out_triangles) const;

private:
    std::shared_ptr<BSDF> _bsdf;

    std::vector<Vector3R> _positions;
    std::vector<Vector3R> _normals;
    std::vector<Vector3R> _uvws;
};

} // namespace cadise