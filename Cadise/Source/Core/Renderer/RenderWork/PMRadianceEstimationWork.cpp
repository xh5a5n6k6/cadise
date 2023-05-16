#include "Core/Renderer/RenderWork/PMRadianceEstimationWork.h"

#include "Core/Film/Film.h"
#include "Core/Renderer/PhotonMapping/PMViewpoint.h"
#include "Core/Surface/BSDF/BSDF.h"
#include "Core/Surface/TransportInfo.h"
#include "Core/SurfaceIntersection.h"
#include "Foundation/Assertion.h"
#include "Math/Constant.h"

#include <cmath>

namespace cadise
{

PMRadianceEstimationWork::PMRadianceEstimationWork(
    const PhotonMap* const    photonMap,
    std::vector<PMViewpoint>* viewpoints,
    Film*                     film,
    const std::size_t         numPhotonPaths,
    const real                alpha) :

    _photonMap(photonMap),
    _viewpoints(viewpoints),
    _film(film),
    _numPhotonPaths(numPhotonPaths),
    _alpha(alpha),
    _workBeginIndex(),
    _workEndIndex()
{
    CS_ASSERT(photonMap);
    CS_ASSERT(viewpoints);
    CS_ASSERT(film);
}

void PMRadianceEstimationWork::work() const
{
    // for each viewpoint, do progressive radiance estimation
    // PPM paper chapter 4
    for (std::size_t i = _workBeginIndex; i < _workEndIndex; ++i)
    {
        PMViewpoint& viewpoint = (*_viewpoints)[i];

        const BSDF*          bsdf                 = viewpoint.bsdf();
        const SurfaceDetail& surfaceDetail        = viewpoint.surfaceDetail();
        const Vector3R&      fromDirection        = viewpoint.fromDirection();
        const Vector2D&      filmPosition         = viewpoint.filmPosition();
        const Spectrum&      throughputImportance = viewpoint.throughputImportance();
        const Spectrum&      emittedRadiance      = viewpoint.emittedRadiance();

        // HACK: add zero radiance for non-hit viewpoint
        if (!bsdf)
        {
            _film->addSampleRadiance(filmPosition, Spectrum(0.0_r));

            continue;
        }

        // radius reduction
        // PPM paper section 4.1
        std::vector<Photon> nearPhotons;
        _photonMap->findWithRange(surfaceDetail.position(), viewpoint.radius(), &nearPhotons);

        const real N          = viewpoint.numPhotons();
        const real M          = static_cast<real>(nearPhotons.size());
        const real newN       = N + _alpha * M;
        const real multiplier = (N + M == 0.0_r) ? 0.0_r : newN / (N + M);

        const real R    = viewpoint.radius();
        const real newR = (multiplier == 0.0_r) ? R : R * std::sqrt(multiplier);

        // flux correction
        // PPM paper section 4.2
        Spectrum TauM(0.0_r);
        {
            const TransportInfo transportInfo(ETransportMode::Importance);

            SurfaceIntersection si;
            si.setSurfaceDetail(surfaceDetail);
            si.setWo(fromDirection);

            for (const auto& photon : nearPhotons)
            {
                si.setWi(photon.fromDirection());

                const Spectrum f = bsdf->evaluate(transportInfo, si);
                if (!f.isZero())
                {
                    TauM.addLocal(f.mul(photon.throughputRadiance()));
                }
            }
        }

        const Spectrum& TauN    = viewpoint.tau();
        const Spectrum  newTauN = TauN.add(TauM).mul(multiplier);

        // radiance evaluation
        // PPM paper section 4.3
        const real kernelFactor
            = 1.0_r / (constant::pi<real> * newR * newR * static_cast<real>(_numPhotonPaths));

        const Spectrum radiance = newTauN.mul(kernelFactor)
            .add(emittedRadiance)
            .mul(throughputImportance);

        _film->addSampleRadiance(filmPosition, radiance);

        // update viewpoint data
        viewpoint.setNumPhotons(newN);
        viewpoint.setRadius(newR);
        viewpoint.setTau(newTauN);
    }
}

void PMRadianceEstimationWork::setWorkBeginEnd(
    const std::size_t beginIndex,
    const std::size_t endIndex)
{
    _workBeginIndex = beginIndex;
    _workEndIndex   = endIndex;
}

} // namespace cadise