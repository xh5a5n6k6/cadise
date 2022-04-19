#pragma once

#include "utility/tDenseBuffer.h"

#include "fundamental/assertion.h"

#include <utility>

namespace cadise
{

template<typename T>
inline TDenseBuffer<T>::TDenseBuffer() = default;

template<typename T>
inline std::size_t TDenseBuffer<T>::add(const T& element)
{
    std::size_t id = _elements.size();
    if (!_reusedIds.empty())
    {
        id = _reusedIds.back();
        _reusedIds.pop_back();
    }

    _idToElementMap[id] = _elements.size();
    _elementToIdMap.push_back(id);

    // Push element last
    _elements.push_back(element);

    return id;
}

template<typename T>
inline T& TDenseBuffer<T>::get(const std::size_t id)
{
    CS_ASSERT(_idToElementMap.contains(id));

    const std::size_t index = _idToElementMap[id];
    return _elements[index];
}

template<typename T>
inline const T& TDenseBuffer<T>::get(const std::size_t id) const
{
    CS_ASSERT(_idToElementMap.contains(id));

    const std::size_t index = _idToElementMap[id];
    return _elements[index];
}

template<typename T>
inline void TDenseBuffer<T>::remove(const std::size_t id)
{
    CS_ASSERT(_idToElementMap.contains(id));

    const std::size_t removeIndex = _idToElementMap[id];
    std::swap(_elements[removeIndex], _elements.back());
    _elements.pop_back();

    // Update elements mapping info
    const std::size_t swapId = _elementToIdMap.back();
    _idToElementMap[swapId]      = removeIndex;
    _elementToIdMap[removeIndex] = swapId;

    _idToElementMap.erase(id);
    _elementToIdMap.pop_back();

    _reusedIds.push_back(id);
}

template<typename T>
inline std::vector<T>::iterator TDenseBuffer<T>::begin()
{
    return _elements.begin();
}

template<typename T>
inline std::vector<T>::iterator TDenseBuffer<T>::end()
{
    return _elements.end();
}

template<typename T>
inline std::vector<T>::const_iterator TDenseBuffer<T>::begin() const
{
    return _elements.begin();
}

template<typename T>
inline std::vector<T>::const_iterator TDenseBuffer<T>::end() const
{
    return _elements.end();
}

} // namespace cadise