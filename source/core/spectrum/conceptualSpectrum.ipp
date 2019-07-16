#pragma once

#include "core/spectrum/conceptualSpectrum.h"

namespace cadise {

template<uint32 Size>
inline ConceptualSpectrum<Size>::ConceptualSpectrum() :
    _values() {
}

template<uint32 Size>
inline ConceptualSpectrum<Size>::ConceptualSpectrum(const real v) :
    _values(v) {
}

template<uint32 Size>
inline ConceptualSpectrum<Size>::ConceptualSpectrum(const ConceptualSpectrum<Size>& cs) :
    _values(cs._values) {
}

template<uint32 Size>
inline ConceptualSpectrum<Size> ConceptualSpectrum<Size>::operator-() const {
    ConceptualSpectrum<Size> result;
    result._values = -cs._values;

    return result;
}

template<uint32 Size>
inline ConceptualSpectrum<Size> ConceptualSpectrum<Size>::operator+(const real s) const {
    ConceptualSpectrum<Size> result;
    result._values = _values + s;

    return result;
}

template<uint32 Size>
inline ConceptualSpectrum<Size> ConceptualSpectrum<Size>::operator-(const real s) const {
    ConceptualSpectrum<Size> result;
    result._values = _values - s;

    return result;
}

template<uint32 Size>
inline ConceptualSpectrum<Size> ConceptualSpectrum<Size>::operator*(const real s) const {
    ConceptualSpectrum<Size> result;
    result._values = _values * s;

    return result;
}

template<uint32 Size>
inline ConceptualSpectrum<Size> ConceptualSpectrum<Size>::operator/(const real s) const {
    ConceptualSpectrum<Size> result;
    result._values = _values / s;

    return result;
}

template<uint32 Size>
inline ConceptualSpectrum<Size> ConceptualSpectrum<Size>::operator+(const ConceptualSpectrum<Size>& cs) const {
    ConceptualSpectrum<Size> result;
    result._values = _values + cs._values;

    return result;
}

template<uint32 Size>
inline ConceptualSpectrum<Size> ConceptualSpectrum<Size>::operator-(const ConceptualSpectrum<Size>& cs) const {
    ConceptualSpectrum<Size> result;
    result._values = _values - cs._values;

    return result;
}

template<uint32 Size>
inline ConceptualSpectrum<Size> ConceptualSpectrum<Size>::operator*(const ConceptualSpectrum<Size>& cs) const {
    ConceptualSpectrum<Size> result;
    result._values = _values * cs._values;

    return result;
}

template<uint32 Size>
inline ConceptualSpectrum<Size> ConceptualSpectrum<Size>::operator/(const ConceptualSpectrum<Size>& cs) const {
    ConceptualSpectrum<Size> result;
    result._values = _values / cs._values;

    return result;
}

template<uint32 Size>
inline ConceptualSpectrum<Size>& ConceptualSpectrum<Size>::operator+=(const real s) {
    _values += s;

    return *this;
}

template<uint32 Size>
inline ConceptualSpectrum<Size>& ConceptualSpectrum<Size>::operator-=(const real s) {
    _values -= s;

    return *this;
}

template<uint32 Size>
inline ConceptualSpectrum<Size>& ConceptualSpectrum<Size>::operator*=(const real s) {
    _values *= s;

    return *this;
}

template<uint32 Size>
inline ConceptualSpectrum<Size>& ConceptualSpectrum<Size>::operator/=(const real s) {
    _values /= s;

    return *this;
}

template<uint32 Size>
inline ConceptualSpectrum<Size>& ConceptualSpectrum<Size>::operator+=(const ConceptualSpectrum<Size>& cs) {
    _values += cs._values;

    return *this;
}

template<uint32 Size>
inline ConceptualSpectrum<Size>& ConceptualSpectrum<Size>::operator-=(const ConceptualSpectrum<Size>& cs) {
    _values -= cs._values;

    return *this;
}

template<uint32 Size>
inline ConceptualSpectrum<Size>& ConceptualSpectrum<Size>::operator*=(const ConceptualSpectrum<Size>& cs) {
    _values *= cs._values;

    return *this;
}

template<uint32 Size>
inline ConceptualSpectrum<Size>& ConceptualSpectrum<Size>::operator/=(const ConceptualSpectrum<Size>& cs) {
    _values /= cs._values;

    return *this;
}

template<uint32 Size>
inline ConceptualSpectrum<Size>& ConceptualSpectrum<Size>::operator=(const ConceptualSpectrum<Size>& cs) {
    _values = cs._values;

    return *this;
}

template<uint32 Size>
inline bool ConceptualSpectrum<Size>::isZero() const {
    return _values.isZero();
}

template<uint32 Size>
inline real ConceptualSpectrum<Size>::sum() const {
    return _values.sum();
}

template<uint32 Size>
inline real ConceptualSpectrum<Size>::average() const {
    return _values.average();
}

} // namespace cadise