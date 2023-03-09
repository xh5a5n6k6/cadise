#include "core/intersector/primitive/triangle.h"

#include "core/integral-tool/sample/positionSample.h"
#include "core/intersector/primitiveInfo.h"
#include "core/ray.h"
#include "core/surfaceDetail.h"
#include "core/texture/mapper/textureMapper.h"
#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/random.h"
#include "math/tAabb3.h"
#include "math/tVector2.h"
#include "math/warp/sampleWarp.h"

namespace cadise
{

Triangle::Triangle(
    const std::shared_ptr<Bsdf>& bsdf, 
    const Vector3R&              vA, 
    const Vector3R&              vB, 
    const Vector3R&              vC) :
    
    Primitive(bsdf), 
    _vA(vA),
    _vB(vB), 
    _vC(vC)
{
    CS_ASSERT(bsdf);

    _eAB = _vB.sub(_vA);
    _eAC = _vC.sub(_vA);

    // TODO: refactor here
    Vector3R N = _eAB.cross(_eAC);
    N = (N.isZero()) ? Vector3R(0.0_r, 1.0_r, 0.0_r) : N.normalize();

    _nA = N;
    _nB = N;
    _nC = N;

    _uvwA = Vector3R(0.0_r, 0.0_r, 0.0_r);
    _uvwB = Vector3R(1.0_r, 0.0_r, 0.0_r);
    _uvwC = Vector3R(0.0_r, 1.0_r, 0.0_r);
}

void Triangle::evaluateBound(AABB3R* const out_bound) const
{
    CS_ASSERT(out_bound);

    AABB3R bound(_vA);
    bound.unionWithLocal(_vB).unionWithLocal(_vC).expandLocal(0.0001_r);

    out_bound->set(bound);
}

bool Triangle::isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const 
{
    Vector3R eAB = _eAB;
    Vector3R eAC = _eAC;

    const Vector3R& D = ray.direction();
    if (D.dot(eAB.cross(eAC)) > 0.0_r) 
    {
        eAB.swap(eAC);
    }

    const Vector3R T = ray.origin().sub(_vA);
    const Vector3R Q = T.cross(eAB);
    const Vector3R P = D.cross(eAC);

    const real denominator = P.dot(eAB);
    if (denominator == 0.0_r)
    {
        return false;
    }

    const real mulFactor = 1.0_r / denominator;
    const real t         = Q.dot(eAC) * mulFactor;
    const real u         = P.dot(T) * mulFactor;
    const real v         = Q.dot(D) * mulFactor;

    if (u < 0.0_r || v < 0.0_r || u + v > 1.0_r) 
    {
        return false;
    }

    if (t < ray.minT() || t > ray.maxT()) 
    {
        return false;
    }

    ray.setMaxT(t);
    primitiveInfo.setPrimitive(this);

    return true;
}

bool Triangle::isOccluded(const Ray& ray) const 
{
    Vector3R eAB = _eAB;
    Vector3R eAC = _eAC;

    const Vector3R& D = ray.direction();
    if (D.dot(eAB.cross(eAC)) > 0.0_r) 
    {
        eAB.swap(eAC);
    }

    const Vector3R T = ray.origin().sub(_vA);
    const Vector3R Q = T.cross(eAB);
    const Vector3R P = D.cross(eAC);

    const real denominator = P.dot(eAB);
    if (denominator == 0.0_r)
    {
        return false;
    }

    const real mulFactor = 1.0_r / denominator;
    const real t         = Q.dot(eAC) * mulFactor;
    const real u         = P.dot(T) * mulFactor;
    const real v         = Q.dot(D) * mulFactor;

    if (u < 0.0_r || v < 0.0_r || u + v > 1.0_r)
    {
        return false;
    }

    if (t < ray.minT() || t > ray.maxT())
    {
        return false;
    }

    return true;
}

void Triangle::evaluateSurfaceDetail(
    const PrimitiveInfo& primitiveInfo, 
    SurfaceDetail* const out_surface) const 
{
    CS_ASSERT(out_surface);

    const Vector3R& P = out_surface->position();

    Vector3R barycentric;
    _positionToBarycentric(P, &barycentric);

    Vector3R Ng;
    Vector3R Ns;
    Vector3R uvw;
    {
        // Ng
        // TODO: refactor here
        Ng = _eAB.cross(_eAC);
        Ng = (Ng.isZero()) ? Vector3R(0.0_r, 1.0_r, 0.0_r) : Ng.normalize();

        // Ns
        Ns = _nA.mul(barycentric.x()).add(
             _nB.mul(barycentric.y())).add(
             _nC.mul(barycentric.z()));

        Ns = (Ns.isZero()) ? Ng : Ns.normalize();

        // uvw
        if (_textureMapper)
        {
            _textureMapper->mappingToUvw(Ng, &uvw);
        }
        else 
        {
            uvw = _uvwA.mul(barycentric.x()).add(
                  _uvwB.mul(barycentric.y())).add(
                  _uvwC.mul(barycentric.z()));
        }
    }

    out_surface->setGeometryNormal(Ng);
    out_surface->setShadingNormal(Ns);
    out_surface->setUvw(uvw);

    /*
        calculate differential geometry properties.

        implementation follows PBRT-v3's solution
        Reference: PBRT-v3 e-book (Triangle Meshes)
    */
    Vector3R dPdU(0.0_r);
    Vector3R dPdV(0.0_r);
    Vector3R dNdU(0.0_r);
    Vector3R dNdV(0.0_r);
    {
        const Vector2R dUVab(_uvwB[0] - _uvwA[0], _uvwB[1] - _uvwA[1]);
        const Vector2R dUVac(_uvwC[0] - _uvwA[0], _uvwC[1] - _uvwA[1]);

        const real determinant = dUVab[0] * dUVac[1] - dUVab[1] * dUVac[0];
        if (determinant != 0.0_r)
        {
            const real rcpDeterminant = 1.0_r / determinant;

            const Vector3R dPab = _eAB;
            const Vector3R dPac = _eAC;
            const Vector3R dNab = _nB.sub(_nA);
            const Vector3R dNac = _nC.sub(_nA);

            dPdU = dPab.mul( dUVac[1]).sub(dPac.mul(dUVab[1])).mul(rcpDeterminant);
            dPdV = dPab.mul(-dUVac[0]).add(dPac.mul(dUVab[0])).mul(rcpDeterminant);
            dNdU = dNab.mul( dUVac[1]).sub(dNac.mul(dUVab[1])).mul(rcpDeterminant);
            dNdV = dNab.mul(-dUVac[0]).add(dNac.mul(dUVab[0])).mul(rcpDeterminant);
        }
    }

    out_surface->setDifferentialGeometry({ dPdU, dPdV, dNdU, dNdV });
}

void Triangle::evaluatePositionSample(PositionSample* const out_sample) const
{
    CS_ASSERT(out_sample);

    const std::array<real, 2> sample = { Random::nextReal(), Random::nextReal() };
    Vector2R sampleSt;
    SampleWarp::uniformTriangleUv(sample, &sampleSt);

    const Vector3R eAB = _eAB;
    const Vector3R eAC = _eAC;
    const Vector3R P   = _vA.add(eAB.mul(sampleSt[0])).add(eAC.mul(sampleSt[1]));

    Vector3R barycentric;
    _positionToBarycentric(P, &barycentric);

    Vector3R Ng = eAB.cross(eAC);
    Ng = (Ng.isZero()) ? Vector3R(0.0_r, 1.0_r, 0.0_r) : Ng.normalize();

    Vector3R Ns = _nA.mul(barycentric.x()).add(
                  _nB.mul(barycentric.y())).add(
                  _nC.mul(barycentric.z()));
    Ns = (Ns.isZero()) ? Ng : Ns;

    const Vector3R uvw = _uvwA.mul(barycentric.x()).add(
                         _uvwB.mul(barycentric.y())).add(
                         _uvwC.mul(barycentric.z()));

    out_sample->setPosition(P);
    out_sample->setGeometryNormal(Ng);
    out_sample->setShadingNormal(Ns);
    out_sample->setUvw(uvw);
    out_sample->setPdfA(this->evaluatePositionPdfA(P));
}

real Triangle::evaluatePositionPdfA(const Vector3R& position) const 
{
    return 1.0_r / this->area();
}

real Triangle::area() const 
{
    return 0.5_r * _eAB.cross(_eAC).length();
}

void Triangle::setNa(const Vector3R& nA)
{
    if (!nA.isZero())
    {
        _nA = nA;
    }
}

void Triangle::setNb(const Vector3R& nB) 
{
    if (!nB.isZero())
    {
        _nB = nB;
    }
}

void Triangle::setNc(const Vector3R& nC)
{
    if (!nC.isZero())
    {
        _nC = nC;
    }
}

void Triangle::setUvwA(const Vector3R& uvwA) 
{
    _uvwA = uvwA;
}

void Triangle::setUvwB(const Vector3R& uvwB)
{
    _uvwB = uvwB;
}

void Triangle::setUvwC(const Vector3R& uvwC)
{
    _uvwC = uvwC;
}

void Triangle::_positionToBarycentric(
    const Vector3R& position, 
    Vector3R* const out_barycentric) const 
{
    CS_ASSERT(out_barycentric);

    const Vector3R v0 = _eAB;
    const Vector3R v1 = _eAC;
    const Vector3R v2 = position.sub(_vA);

    const real d00 = v0.dot(v0);
    const real d01 = v0.dot(v1);
    const real d11 = v1.dot(v1);
    const real d20 = v2.dot(v0);
    const real d21 = v2.dot(v1);
    const real denominator = d00 * d11 - d01 * d01;

    if (denominator == 0.0_r) 
    {
        out_barycentric->set(0.0_r);
    }
    else
    {
        const real mulFactor = 1.0_r / denominator;
        
        const real u = ( d11 * d20 - d01 * d21) * mulFactor;
        const real v = (-d01 * d20 + d00 * d21) * mulFactor;

        out_barycentric->set(
            1.0_r - u - v, 
            u, 
            v);
    }
}

} // namespace cadise