#pragma once

#include "math/vector.h"

namespace cadise {

// TODO: refactor as CRTP (curiously recurring template pattern)
template<std::size_t N>
class ConceptualSpectrum {
protected:
    using SpectralArray = Vector<real, N>;

public:
    ConceptualSpectrum();
    explicit ConceptualSpectrum(const real v);
    explicit ConceptualSpectrum(const SpectralArray& sa);
    ConceptualSpectrum(const ConceptualSpectrum& cs);

    virtual ~ConceptualSpectrum();

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
    bool hasNegative() const;
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