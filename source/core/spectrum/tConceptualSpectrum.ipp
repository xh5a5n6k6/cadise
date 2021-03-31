#pragma once

#include "core/spectrum/tConceptualSpectrum.h"

namespace cadise {

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>::TConceptualSpectrum() :
    _values() {
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>::TConceptualSpectrum(const T value) :
    _values(value) {
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>::TConceptualSpectrum(const TArithmeticArray<T, N>& value) :
    _values(value) {
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>::TConceptualSpectrum(const TConceptualSpectrum<T, N>& other) = default;

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>::~TConceptualSpectrum() = default;

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::operator + (const T rhs) const {
    return this->add(rhs);
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::operator - (const T rhs) const {
    return this->sub(rhs);
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::operator * (const T rhs) const {
    return this->mul(rhs);
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::operator / (const T rhs) const {
    return this->div(rhs);
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::operator + (const TConceptualSpectrum<T, N>& rhs) const {
    return this->add(rhs);
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::operator - (const TConceptualSpectrum<T, N>& rhs) const {
    return this->sub(rhs);
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::operator * (const TConceptualSpectrum<T, N>& rhs) const {
    return this->mul(rhs);
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::operator / (const TConceptualSpectrum<T, N>& rhs) const {
    return this->div(rhs);
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::add(const TConceptualSpectrum& rhs) const {
    return TConceptualSpectrum<T, N>(_values.add(rhs._values));
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::add(const T rhs) const {
    return TConceptualSpectrum<T, N>(_values.add(rhs));
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>& TConceptualSpectrum<T, N>::addLocal(const TConceptualSpectrum<T, N>& rhs) {
    _values.addLocal(rhs._values);

    return *this;
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>& TConceptualSpectrum<T, N>::addLocal(const T rhs) {
    _values.addLocal(rhs);

    return *this;
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::sub(const TConceptualSpectrum& rhs) const {
    return TConceptualSpectrum<T, N>(_values.sub(rhs._values));
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::sub(const T rhs) const {
    return TConceptualSpectrum<T, N>(_values.sub(rhs));
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>& TConceptualSpectrum<T, N>::subLocal(const TConceptualSpectrum<T, N>& rhs) {
    _values.subLocal(rhs._values);

    return *this;
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>& TConceptualSpectrum<T, N>::subLocal(const T rhs) {
    _values.subLocal(rhs);

    return *this;
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::mul(const TConceptualSpectrum& rhs) const {
    return TConceptualSpectrum<T, N>(_values.mul(rhs._values));
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::mul(const T rhs) const {
    return TConceptualSpectrum<T, N>(_values.mul(rhs));
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>& TConceptualSpectrum<T, N>::mulLocal(const TConceptualSpectrum<T, N>& rhs) {
    _values.mulLocal(rhs._values);

    return *this;
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>& TConceptualSpectrum<T, N>::mulLocal(const T rhs) {
    _values.mulLocal(rhs);

    return *this;
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::div(const TConceptualSpectrum& rhs) const {
    return TConceptualSpectrum<T, N>(_values.div(rhs._values));
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::div(const T rhs) const {
    return TConceptualSpectrum<T, N>(_values.div(rhs));
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>& TConceptualSpectrum<T, N>::divLocal(const TConceptualSpectrum<T, N>& rhs) {
    _values.divLocal(rhs._values);

    return *this;
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>& TConceptualSpectrum<T, N>::divLocal(const T rhs) {
    _values.divLocal(rhs);

    return *this;
}

template<typename T, std::size_t N>
inline bool TConceptualSpectrum<T, N>::isZero() const {
    return _values.isZero();
}

template<typename T, std::size_t N>
inline bool TConceptualSpectrum<T, N>::hasNegative() const {
    return _values.hasNegative();
}

template<typename T, std::size_t N>
inline bool TConceptualSpectrum<T, N>::hasNaN() const {
    return _values.hasNaN();
}

template<typename T, std::size_t N>
inline bool TConceptualSpectrum<T, N>::hasInfinite() const {
    return _values.hasInfinite();
}

template<typename T, std::size_t N>
inline T TConceptualSpectrum<T, N>::sum() const {
    return _values.sum();
}

template<typename T, std::size_t N>
inline T TConceptualSpectrum<T, N>::average() const {
    return _values.average();
}

template<typename T, std::size_t N>
inline T TConceptualSpectrum<T, N>::maxComponent() const {
    return _values.maxComponent();
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::squared() const {
    return TConceptualSpectrum<T, N>(_values.squared());
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>& TConceptualSpectrum<T, N>::squaredLocal() {
    _values.squaredLocal();
    
    return *this;
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N> TConceptualSpectrum<T, N>::complement() const {
    return TConceptualSpectrum<T, N>(_values.complement());
}

template<typename T, std::size_t N>
inline TConceptualSpectrum<T, N>& TConceptualSpectrum<T, N>::complementLocal() {
    _values.complementLocal();

    return *this;
}

template<typename T, std::size_t N>
inline void TConceptualSpectrum<T, N>::set(const TConceptualSpectrum& other) {
    _values = other._values;
}

template<typename T, std::size_t N>
inline void TConceptualSpectrum<T, N>::set(const TArithmeticArray<T, N>& value) {
    _values = value;
}

template<typename T, std::size_t N>
inline void TConceptualSpectrum<T, N>::set(const T value) {
    _values.set(value);
}

} // namespace cadise