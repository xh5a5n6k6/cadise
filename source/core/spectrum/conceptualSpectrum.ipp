#pragma once

#include "core/spectrum/conceptualSpectrum.h"

namespace cadise {

template<std::size_t N>
inline ConceptualSpectrum<N>::ConceptualSpectrum() :
    _values() {
}

template<std::size_t N>
inline ConceptualSpectrum<N>::ConceptualSpectrum(const real v) :
    _values(v) {
}

template<std::size_t N>
inline ConceptualSpectrum<N>::ConceptualSpectrum(const SpectralArray& as) :
    _values(as) {
}

template<std::size_t N>
inline ConceptualSpectrum<N>::ConceptualSpectrum(const ConceptualSpectrum<N>& cs) :
    _values(cs._values) {
}

template<std::size_t N>
inline ConceptualSpectrum<N> ConceptualSpectrum<N>::operator-() const {
    ConceptualSpectrum<N> result;
    result._values = -_values;

    return result;
}

template<std::size_t N>
inline ConceptualSpectrum<N> ConceptualSpectrum<N>::operator+(const real s) const {
    ConceptualSpectrum<N> result;
    result._values = _values + s;

    return result;
}

template<std::size_t N>
inline ConceptualSpectrum<N> ConceptualSpectrum<N>::operator-(const real s) const {
    ConceptualSpectrum<N> result;
    result._values = _values - s;

    return result;
}

template<std::size_t N>
inline ConceptualSpectrum<N> ConceptualSpectrum<N>::operator*(const real s) const {
    ConceptualSpectrum<N> result;
    result._values = _values * s;

    return result;
}

template<std::size_t N>
inline ConceptualSpectrum<N> ConceptualSpectrum<N>::operator/(const real s) const {
    ConceptualSpectrum<N> result;
    result._values = _values / s;

    return result;
}

template<std::size_t N>
inline ConceptualSpectrum<N> ConceptualSpectrum<N>::operator+(const ConceptualSpectrum<N>& cs) const {
    ConceptualSpectrum<N> result;
    result._values = _values + cs._values;

    return result;
}

template<std::size_t N>
inline ConceptualSpectrum<N> ConceptualSpectrum<N>::operator-(const ConceptualSpectrum<N>& cs) const {
    ConceptualSpectrum<N> result;
    result._values = _values - cs._values;

    return result;
}

template<std::size_t N>
inline ConceptualSpectrum<N> ConceptualSpectrum<N>::operator*(const ConceptualSpectrum<N>& cs) const {
    ConceptualSpectrum<N> result;
    result._values = _values * cs._values;

    return result;
}

template<std::size_t N>
inline ConceptualSpectrum<N> ConceptualSpectrum<N>::operator/(const ConceptualSpectrum<N>& cs) const {
    ConceptualSpectrum<N> result;
    result._values = _values / cs._values;

    return result;
}

template<std::size_t N>
inline ConceptualSpectrum<N>& ConceptualSpectrum<N>::operator+=(const real s) {
    _values += s;

    return *this;
}

template<std::size_t N>
inline ConceptualSpectrum<N>& ConceptualSpectrum<N>::operator-=(const real s) {
    _values -= s;

    return *this;
}

template<std::size_t N>
inline ConceptualSpectrum<N>& ConceptualSpectrum<N>::operator*=(const real s) {
    _values *= s;

    return *this;
}

template<std::size_t N>
inline ConceptualSpectrum<N>& ConceptualSpectrum<N>::operator/=(const real s) {
    _values /= s;

    return *this;
}

template<std::size_t N>
inline ConceptualSpectrum<N>& ConceptualSpectrum<N>::operator+=(const ConceptualSpectrum<N>& cs) {
    _values += cs._values;

    return *this;
}

template<std::size_t N>
inline ConceptualSpectrum<N>& ConceptualSpectrum<N>::operator-=(const ConceptualSpectrum<N>& cs) {
    _values -= cs._values;

    return *this;
}

template<std::size_t N>
inline ConceptualSpectrum<N>& ConceptualSpectrum<N>::operator*=(const ConceptualSpectrum<N>& cs) {
    _values *= cs._values;

    return *this;
}

template<std::size_t N>
inline ConceptualSpectrum<N>& ConceptualSpectrum<N>::operator/=(const ConceptualSpectrum<N>& cs) {
    _values /= cs._values;

    return *this;
}

template<std::size_t N>
inline ConceptualSpectrum<N>& ConceptualSpectrum<N>::operator=(const ConceptualSpectrum<N>& cs) {
    _values = cs._values;

    return *this;
}

template<std::size_t N>
inline bool ConceptualSpectrum<N>::isZero() const {
    return _values.isZero();
}

template<std::size_t N>
inline bool ConceptualSpectrum<N>::hasNaN() const {
    return _values.hasNaN();
}

template<std::size_t N>
inline bool ConceptualSpectrum<N>::hasInfinite() const {
    return _values.hasInfinite();
}

template<std::size_t N>
inline real ConceptualSpectrum<N>::sum() const {
    return _values.sum();
}

template<std::size_t N>
inline real ConceptualSpectrum<N>::average() const {
    return _values.average();
}

template<std::size_t N>
inline real ConceptualSpectrum<N>::maxComponent() const {
    return _values.maxComponent();
}

template<std::size_t N>
inline ConceptualSpectrum<N> ConceptualSpectrum<N>::square() const {
    return ConceptualSpectrum<N>(_values.square());
}

template<std::size_t N>
inline ConceptualSpectrum<N> ConceptualSpectrum<N>::complement() const {
    return ConceptualSpectrum<N>(_values.complement());
}

} // namespace cadise