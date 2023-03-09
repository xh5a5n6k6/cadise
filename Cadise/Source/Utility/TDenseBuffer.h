#pragma once

#include <unordered_map>
#include <vector>

namespace cadise
{

template<typename T>
class TDenseBuffer
{
public:
    TDenseBuffer();

    std::size_t add(const T& element);

    T& get(const std::size_t id);
    const T& get(const std::size_t id) const;

    void remove(const std::size_t id);

    std::vector<T>::iterator begin();
    std::vector<T>::iterator end();
    std::vector<T>::const_iterator begin() const;
    std::vector<T>::const_iterator end() const;

private:
    std::unordered_map<std::size_t, std::size_t> _idToElementMap;
    std::vector<std::size_t> _elementToIdMap;
    std::vector<std::size_t> _reusedIds;

    std::vector<T> _elements;
};

} // namespace cadise

#include "utility/tDenseBuffer.ipp"