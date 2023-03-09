#pragma once

#include "math/tVector3.h"

#include <memory>
#include <vector>

namespace cadise { class Bsdf; }
namespace cadise { class Primitive; }

namespace cadise 
{

/*
    
*/
class TriangleBuffer
{
public:
    TriangleBuffer(
        const std::shared_ptr<Bsdf>& bsdf, 
        const std::vector<Vector3R>& positions,
        const std::vector<Vector3R>& normals, 
        const std::vector<Vector3R>& uvws);

    void transformToTriangles(
        std::vector<std::shared_ptr<Primitive>>* const out_triangles) const;

private:
    std::shared_ptr<Bsdf> _bsdf;

    std::vector<Vector3R> _positions;
    std::vector<Vector3R> _normals;
    std::vector<Vector3R> _uvws;
};

} // namespace cadise