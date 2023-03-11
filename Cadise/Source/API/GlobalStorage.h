#pragma once

#include "Math/Type/PrimitiveType.h"
#include "Utility/TDenseBuffer.h"

#include <memory>
#include <mutex>


namespace cadise
{

class GlobalStorage
{
public:
    template<typename T>
    static uint64 createResource();

    template<typename T>
    static void deleteResource(const uint64 resourceId);

    template<typename T>
    static std::shared_ptr<T> getResource(const uint64 resourceId);

private:
    template<typename T>
    static TDenseBuffer<std::shared_ptr<T>>& _resources();

    static std::mutex& _getMutex();
};

// template header implementation

template<typename T>
inline uint64 GlobalStorage::createResource()
{
    uint64 id = 0;
    {
        std::lock_guard<std::mutex> lock(_getMutex());

        auto& resources = GlobalStorage::_resources<T>();
        id = resources.add(std::make_shared<T>());
    }

    return id;
}

template<typename T>
inline void GlobalStorage::deleteResource(const uint64 resourceId)
{
    std::lock_guard<std::mutex> lock(_getMutex());

    auto& resources = GlobalStorage::_resources<T>();
    resources.remove(resourceId);
}

template<typename T>
inline std::shared_ptr<T> GlobalStorage::getResource(const uint64 resourceId)
{
    std::shared_ptr<T> resource = nullptr;
    {
        std::lock_guard<std::mutex> lock(_getMutex());

        auto& resources = GlobalStorage::_resources<T>();
        resource = resources.get(resourceId);
    }
    
    return resource;
}

template<typename T>
inline TDenseBuffer<std::shared_ptr<T>>& GlobalStorage::_resources()
{
    static TDenseBuffer<std::shared_ptr<T>> resources;

    return resources;
}

} // namespace cadise