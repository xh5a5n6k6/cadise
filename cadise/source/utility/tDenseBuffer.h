#pragma once

#include "math/type/fundamentalType.h"

#include <stack>
#include <unordered_map>
#include <vector>

namespace cadise
{

template<typename T>
class TDenseBuffer
{
public:
    TDenseBuffer();

    uint32 addElement(const T& element);

    T& getElement(const uint32 elementId);
    const T& getElement(const uint32 elementId) const;

    void removeElement(const uint32 elementId);

private:
    std::unordered_map<uint32, std::size_t> _idToIndexMap;
    std::unordered_map<std::size_t, uint32> _indexToIdMap;
    std::stack<uint32> _reusedIdStack;

    std::vector<T> _elements;
};

} // namespace cadise

#include "utility/tDenseBuffer.ipp"