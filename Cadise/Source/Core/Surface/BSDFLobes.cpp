#include "Core/Surface/BSDFLobes.h"

namespace cadise 
{

BSDFLobes::BSDFLobes() = default;

BSDFLobes::BSDFLobes(const BSDFLobes& other) :
    _lobes(other._lobes) 
{}

BSDFLobes::BSDFLobes(const Lobes& lobes) 
{
    _lobes = _getAllLobes(lobes);
}

BSDFLobes BSDFLobes::operator | (const BSDFLobes& rhs) const 
{
    BSDFLobes result;
    result._lobes = _lobes | rhs._lobes;

    return result;
}

bool BSDFLobes::isEmpty() const
{
    return _lobes == static_cast<uint32>(0);
}

bool BSDFLobes::hasNone(const Lobes& lobes) const 
{
    return (_lobes & _getAllLobes(lobes)) == static_cast<uint32>(0);
}

bool BSDFLobes::hasExactly(const Lobes& lobes) const
{
    return _lobes == _getAllLobes(lobes);
}

bool BSDFLobes::hasAtLeastOne(const Lobes& lobes) const 
{
    return (_lobes & _getAllLobes(lobes)) != static_cast<uint32>(0);
}

bool BSDFLobes::hasOnlyAtLeastOne(const Lobes& lobes) const
{
    const uint32 allLobes = _getAllLobes(lobes);

    return (_lobes & allLobes) != static_cast<uint32>(0) &&
           (_lobes & (~allLobes)) == static_cast<uint32>(0);
}

uint32 BSDFLobes::rawLobes() const 
{
    return _lobes;
}

uint32 BSDFLobes::_getAllLobes(const Lobes& lobes) const
{
    uint32 allLobes = 0;

    for (auto& checkLobe : lobes)
    {
        allLobes |= static_cast<uint32>(checkLobe);
    }

    return allLobes;
}

} // namespace cadise