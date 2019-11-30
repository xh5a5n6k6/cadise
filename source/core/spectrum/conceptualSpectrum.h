#pragma once

#include "math/vector.h"

namespace cadise {

template<std::size_t N>
class ConceptualSpectrum {
protected:
    using SpectralArray = Vector<real, N>;

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
    bool hasNaN() const;
    bool hasInfinite() const;
    real sum() const;
    real average() const;
    real maxComponent() const;

    ConceptualSpectrum square() const;
    ConceptualSpectrum complement() const;

protected:
    SpectralArray _values;
};

// template header implementation

template<std::size_t N>
inline ConceptualSpectrum<N> operator+(const real s, const ConceptualSpectrum<N>& cs) {
    return cs + s;
}

template<std::size_t N>
inline ConceptualSpectrum<N> operator*(const real s, const ConceptualSpectrum<N>& cs) {
    return cs * s;
}

} // namespace cadise

#include "core/spectrum/conceptualSpectrum.ipp"