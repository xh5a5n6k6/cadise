#pragma once

#include "math/vector.h"

namespace cadise {

template<uint32 Size>
class ConceptualSpectrum {
public:
    ConceptualSpectrum();
    ConceptualSpectrum(const real v);
    ConceptualSpectrum(const ConceptualSpectrum& cs);

    ConceptualSpectrum operator-() const;
    ConceptualSpectrum operator+(const real s) const;
    ConceptualSpectrum operator-(const real s) const;
    ConceptualSpectrum operator*(const real s) const;
    ConceptualSpectrum operator/(const real s) const;
    ConceptualSpectrum operator+(const ConceptualSpectrum& cs) const;
    ConceptualSpectrum operator-(const ConceptualSpectrum& cs) const;
    ConceptualSpectrum operator*(const ConceptualSpectrum& cs) const;
    ConceptualSpectrum operator/(const ConceptualSpectrum& cs) const;
    ConceptualSpectrum& operator+=(const real s);
    ConceptualSpectrum& operator-=(const real s);
    ConceptualSpectrum& operator*=(const real s);
    ConceptualSpectrum& operator/=(const real s);
    ConceptualSpectrum& operator+=(const ConceptualSpectrum& cs);
    ConceptualSpectrum& operator-=(const ConceptualSpectrum& cs);
    ConceptualSpectrum& operator*=(const ConceptualSpectrum& cs);
    ConceptualSpectrum& operator/=(const ConceptualSpectrum& cs);
    ConceptualSpectrum& operator=(const ConceptualSpectrum& cs);

    bool isZero() const;
    real sum() const;
    real average() const;

protected:
    using SpectralArray = Vector<real, Size>;
    
    SpectralArray _values;
};

} // namespace cadise

#include "core/spectrum/conceptualSpectrum.ipp"