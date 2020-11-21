#pragma once

#include "core/spectrum/tConceptualSpectrum.h"

namespace cadise {

template<std::size_t N>
inline TConceptualSpectrum<N>::TConceptualSpectrum() :
    _values() {
}

template<std::size_t N>
inline TConceptualSpectrum<N>::TConceptualSpectrum(const real v) :
    _values(v) {
}

template<std::size_t N>
inline TConceptualSpectrum<N>::TConceptualSpectrum(const SpectralArray& as) :
    _values(as) {
}

template<std::size_t N>
inline TConceptualSpectrum<N>::TConceptualSpectrum(const TConceptualSpectrum<N>& cs) = default;

template<std::size_t N>
inline TConceptualSpectrum<N>::~TConceptualSpectrum() = default;

template<std::size_t N>
inline TConceptualSpectrum<N> TConceptualSpectrum<N>::operator - () const {
    TConceptualSpectrum<N> result;
    result._values = -_values;

    return result;
}

template<std::size_t N>
inline TConceptualSpectrum<N> TConceptualSpectrum<N>::operator + (const real s) const {
    TConceptualSpectrum<N> result;
    result._values = _values + s;

    return result;
}

template<std::size_t N>
inline TConceptualSpectrum<N> TConceptualSpectrum<N>::operator - (const real s) const {
    TConceptualSpectrum<N> result;
    result._values = _values - s;

    return result;
}

template<std::size_t N>
inline TConceptualSpectrum<N> TConceptualSpectrum<N>::operator * (const real s) const {
    TConceptualSpectrum<N> result;
    result._values = _values * s;

    return result;
}

template<std::size_t N>
inline TConceptualSpectrum<N> TConceptualSpectrum<N>::operator / (const real s) const {
    TConceptualSpectrum<N> result;
    result._values = _values / s;

    return result;
}

template<std::size_t N>
inline TConceptualSpectrum<N> TConceptualSpectrum<N>::operator + (const TConceptualSpectrum<N>& cs) const {
    TConceptualSpectrum<N> result;
    result._values = _values + cs._values;

    return result;
}

template<std::size_t N>
inline TConceptualSpectrum<N> TConceptualSpectrum<N>::operator - (const TConceptualSpectrum<N>& cs) const {
    TConceptualSpectrum<N> result;
    result._values = _values - cs._values;

    return result;
}

template<std::size_t N>
inline TConceptualSpectrum<N> TConceptualSpectrum<N>::operator * (const TConceptualSpectrum<N>& cs) const {
    TConceptualSpectrum<N> result;
    result._values = _values * cs._values;

    return result;
}

template<std::size_t N>
inline TConceptualSpectrum<N> TConceptualSpectrum<N>::operator / (const TConceptualSpectrum<N>& cs) const {
    TConceptualSpectrum<N> result;
    result._values = _values / cs._values;

    return result;
}

template<std::size_t N>
inline TConceptualSpectrum<N>& TConceptualSpectrum<N>::operator += (const real s) {
    _values += s;

    return *this;
}

template<std::size_t N>
inline TConceptualSpectrum<N>& TConceptualSpectrum<N>::operator -= (const real s) {
    _values -= s;

    return *this;
}

template<std::size_t N>
inline TConceptualSpectrum<N>& TConceptualSpectrum<N>::operator *= (const real s) {
    _values *= s;

    return *this;
}

template<std::size_t N>
inline TConceptualSpectrum<N>& TConceptualSpectrum<N>::operator /= (const real s) {
    _values /= s;

    return *this;
}

template<std::size_t N>
inline TConceptualSpectrum<N>& TConceptualSpectrum<N>::operator += (const TConceptualSpectrum<N>& cs) {
    _values += cs._values;

    return *this;
}

template<std::size_t N>
inline TConceptualSpectrum<N>& TConceptualSpectrum<N>::operator -= (const TConceptualSpectrum<N>& cs) {
    _values -= cs._values;

    return *this;
}

template<std::size_t N>
inline TConceptualSpectrum<N>& TConceptualSpectrum<N>::operator *= (const TConceptualSpectrum<N>& cs) {
    _values *= cs._values;

    return *this;
}

template<std::size_t N>
inline TConceptualSpectrum<N>& TConceptualSpectrum<N>::operator /= (const TConceptualSpectrum<N>& cs) {
    _values /= cs._values;

    return *this;
}

template<std::size_t N>
inline TConceptualSpectrum<N>& TConceptualSpectrum<N>::operator = (const TConceptualSpectrum<N>& cs) {
    _values = cs._values;

    return *this;
}

template<std::size_t N>
inline bool TConceptualSpectrum<N>::isZero() const {
    return _values.isZero();
}

template<std::size_t N>
inline bool TConceptualSpectrum<N>::hasNegative() const {
    return _values.hasNegative();
}

template<std::size_t N>
inline bool TConceptualSpectrum<N>::hasNaN() const {
    return _values.hasNaN();
}

template<std::size_t N>
inline bool TConceptualSpectrum<N>::hasInfinite() const {
    return _values.hasInfinite();
}

template<std::size_t N>
inline real TConceptualSpectrum<N>::sum() const {
    return _values.sum();
}

template<std::size_t N>
inline real TConceptualSpectrum<N>::average() const {
    return _values.average();
}

template<std::size_t N>
inline real TConceptualSpectrum<N>::maxComponent() const {
    return _values.maxComponent();
}

template<std::size_t N>
inline TConceptualSpectrum<N> TConceptualSpectrum<N>::square() const {
    return TConceptualSpectrum<N>(_values.square());
}

template<std::size_t N>
inline TConceptualSpectrum<N> TConceptualSpectrum<N>::complement() const {
    return TConceptualSpectrum<N>(_values.complement());
}

} // namespace cadise