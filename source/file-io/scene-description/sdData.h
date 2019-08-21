#pragma once

#include "core/spectrum/spectrum.h"

#include "file-io/scene-description/sdClassType.h"
#include "file-io/scene-description/sdDataUnit.h"

#include "math/vector.h"

#include <functional>
#include <map>
#include <vector>

namespace cadise {

template<typename T>
class Texture;

class SdData {
public:
    SdData();

    void addBool(
        const std::string_view& name,
        std::unique_ptr<bool[]> value,
        const int32 valueNumber);
    void addReal(
        const std::string_view& name, 
        std::unique_ptr<real[]> value,
        const int32 valueNumber);
    void addInt32(
        const std::string_view& name,
        std::unique_ptr<int32[]> value,
        const int32 valueNumber);
    void addVector3R(
        const std::string_view& name,
        std::unique_ptr<Vector3R[]> value,
        const int32 valueNumber);
    void addString(
        const std::string_view& name,
        std::unique_ptr<std::string_view[]> value,
        const int32 valueNumber);

    bool     findBool(
        const std::string_view& name, const bool      defaultValue = false     ) const;
    real     findReal (
        const std::string_view& name, const real      defaultValue = 0.0_r     ) const;
    int32    findInt32(
        const std::string_view& name, const int32     defaultValue = 0         ) const;
    Vector3R findVector3r(
        const std::string_view& name, const Vector3R& defaultValue = Vector3R()) const;
    std::string_view findString(
        const std::string_view& name, const std::string_view& defaultValue = "") const;

    const real*     findRealArray(
        const std::string_view& name) const;
    const Vector3R* findVector3rArray(
        const std::string_view& name) const;

    std::shared_ptr<Texture<real>> getRealTexture(
        const std::string_view& name, 
        const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures) const;
    std::shared_ptr<Texture<Spectrum>> getSpectrumTexture(
        const std::string_view& name,
        const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) const;

    SdClassType classType() const;

    void setClassType(const std::string_view& classType);
    void setClassType(const SdClassType& classType);

private:
    template<typename T>
    T _findData(
        const std::string_view& name, 
        const T& defaultValue,
        const std::vector<std::shared_ptr<SdDataUnit<T>>>& dataset) const;

    template<typename T>
    const T* _findDataArray(
        const std::string_view& name,
        const std::vector<std::shared_ptr<SdDataUnit<T>>>& dataset) const;

    SdClassType _classType;

    std::vector<std::shared_ptr<SdDataUnit<bool>>>             _bools;
    std::vector<std::shared_ptr<SdDataUnit<real>>>             _reals;
    std::vector<std::shared_ptr<SdDataUnit<int32>>>            _int32s;
    std::vector<std::shared_ptr<SdDataUnit<Vector3R>>>         _vector3rs;
    std::vector<std::shared_ptr<SdDataUnit<std::string_view>>> _strings;
};

// template header implementation

template<typename T>
inline T SdData::_findData(
    const std::string_view& name,
    const T& defaultValue,
    const std::vector<std::shared_ptr<SdDataUnit<T>>>& dataset) const {

    for (uint64 index = 0; index < dataset.size(); index++) {
        if (!dataset[index]->variableName().compare(name)) {
            return dataset[index]->value()[0];
        }
    }

    return defaultValue;
}

template<typename T>
inline const T* SdData::_findDataArray(
    const std::string_view& name,
    const std::vector<std::shared_ptr<SdDataUnit<T>>>& dataset) const {

    for (uint64 index = 0; index < dataset.size(); index++) {
        if (!dataset[index]->variableName().compare(name)) {
            return dataset[index]->value().get();
        }
    }

    return nullptr;
}

} // namespace cadise