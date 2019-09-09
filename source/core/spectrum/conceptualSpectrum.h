#pragma once

#include "math/vector.h"

namespace cadise {

template<uint32 Size>
class ConceptualSpectrum {
protected:
    using SpectralArray = Vector<real, Size>;

public:
    ConceptualSpectrum();
    ConceptualSpectrum(const real v);
    ConceptualSpectrum(const SpectralArray& sa);
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
    real maxComponent() const;

    ConceptualSpectrum complement() const;

protected:
    SpectralArray _values;
};

// template header implementation

template<uint32 Size>
inline ConceptualSpectrum<Size> operator+(const real s, const ConceptualSpectrum<Size>& cs) {
    return cs + s;
}

template<uint32 Size>
inline ConceptualSpectrum<Size> operator*(const real s, const ConceptualSpectrum<Size>& cs) {
    return cs * s;
}

} // namespace cadise

#include "core/spectrum/conceptualSpectrum.ipp"