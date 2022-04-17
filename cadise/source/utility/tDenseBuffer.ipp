#pragma once

#include "utility/tDenseBuffer.h"

#include "fundamental/assertion.h"

#include <utility>

namespace cadise
{

template<typename T>
inline TDenseBuffer<T>::TDenseBuffer() = default;

template<typename T>
inline uint32 TDenseBuffer<T>::addElement(const T& element)
{
    uint32 id = static_cast<uint32>(_elements.size());
    if (!_reusedIdStack.empty())
    {
        id = _reusedIdStack.top();
        _reusedIdStack.pop();
    }

    _idToIndexMap[id]               = _elements.size();
    _indexToIdMap[_elements.size()] = id;

    // Push element last
    _elements.push_back(element);

    return id;
}

template<typename T>
inline T& TDenseBuffer<T>::getElement(const uint32 elementId)
{
    CS_ASSERT(_idToIndexMap.contains(elementId));

    const std::size_t index = _idToIndexMap[elementId];
    return _elements[index];
}

template<typename T>
inline const T& TDenseBuffer<T>::getElement(const uint32 elementId) const
{
    CS_ASSERT(_idToIndexMap.contains(elementId));

    const std::size_t index = _idToIndexMap[elementId];
    return _elements[index];
}

template<typename T>
inline void TDenseBuffer<T>::removeElement(const uint32 elementId)
{
    CS_ASSERT(_idToIndexMap.contains(elementId));

    const std::size_t removeIndex = _idToIndexMap[elementId];
    std::swap(_elements[removeIndex], _elements.back());
    _elements.pop_back();

    // Update elements mapping info
    const uint32 id = _indexToIdMap[_elements.size()];
    _idToIndexMap[id]          = removeIndex;
    _indexToIdMap[removeIndex] = id;

    _idToIndexMap.erase(elementId);
    _indexToIdMap.erase(_elements.size());

    _reusedIdStack.push(elementId);
}

} // namespace cadise