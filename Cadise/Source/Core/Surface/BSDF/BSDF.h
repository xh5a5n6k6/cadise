#pragma once

#include "Core/Surface/BSDFComponents.h"
#include "Core/Surface/BSDFLobes.h"
#include "Core/Spectrum/Spectrum.h"

// forward declaration
namespace cadise
{
    class BSDFSample;
    class SurfaceIntersection;
    class TransportInfo;
}

namespace cadise 
{

class BSDF 
{
public:
    BSDF();
    explicit BSDF(const BSDFLobes& lobes);
    BSDF(const BSDFLobes& lobes, const BSDFComponents components);

    virtual ~BSDF();

    virtual Spectrum evaluate(
        const TransportInfo&       info,
        const SurfaceIntersection& si) const = 0;

    virtual void evaluateSample(
        const TransportInfo&       info,
        const SurfaceIntersection& si,
        BSDFSample* const          out_sample) const = 0;

    virtual real evaluatePdfW(
        const TransportInfo&       info,
        const SurfaceIntersection& si) const = 0;

    virtual ELobe lobe(const BSDFComponents component) const = 0;

    const BSDFLobes& lobes() const;
    BSDFComponents components() const;

protected:
    BSDFLobes      _lobes;
    BSDFComponents _components;
};

} // namespace cadise