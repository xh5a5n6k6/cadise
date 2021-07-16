#include "core/surface/bsdfLobes.h"

namespace cadise {

BsdfLobes::BsdfLobes() = default;

BsdfLobes::BsdfLobes(const BsdfLobes& other) :
    _lobes(other._lobes) {
}

BsdfLobes::BsdfLobes(const Lobes& lobes) {
    _lobes = _getAllLobes(lobes);
}

BsdfLobes BsdfLobes::operator | (const BsdfLobes& rhs) const {
    BsdfLobes result;
    result._lobes = _lobes | rhs._lobes;

    return result;
}

bool BsdfLobes::isEmpty() const {
    return _lobes == static_cast<uint32>(0);
}

bool BsdfLobes::hasNone(const Lobes& lobes) const {
    return (_lobes & _getAllLobes(lobes)) == static_cast<uint32>(0);
}

bool BsdfLobes::hasExactly(const Lobes& lobes) const {
    return _lobes == _getAllLobes(lobes);
}

bool BsdfLobes::hasAtLeastOne(const Lobes& lobes) const {
    return (_lobes & _getAllLobes(lobes)) != static_cast<uint32>(0);
}

bool BsdfLobes::hasOnlyAtLeastOne(const Lobes& lobes) const {
    const uint32 allLobes = _getAllLobes(lobes);

    return (_lobes & allLobes) != static_cast<uint32>(0) &&
           (_lobes & (~allLobes)) == static_cast<uint32>(0);
}

uint32 BsdfLobes::rawLobes() const {
    return _lobes;
}

uint32 BsdfLobes::_getAllLobes(const Lobes& lobes) const {
    uint32 allLobes = 0;

    for (auto& checkLobe : lobes) {
        allLobes |= static_cast<uint32>(checkLobe);
    }

    return allLobes;
}

} // namespace cadise