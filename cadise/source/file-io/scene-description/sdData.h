#pragma once

#include "core/spectrum/spectrum.h"
#include "file-io/scene-description/eSdClassType.h"
#include "file-io/scene-description/tSdDataUnit.h"
#include "math/type/mapType.h"
#include "math/tVector3.h"

#include <memory>
#include <type_traits>
#include <vector>

// forward declaration
namespace cadise 
{ 
    template<typename T> 
    class TTexture; 
}

namespace cadise
{

class SdData 
{
public:
    SdData();

    void addBool(
        const std::string&      name,
        std::unique_ptr<bool[]> value,
        const std::size_t       numValues);
    void addReal(
        const std::string&      name, 
        std::unique_ptr<real[]> value,
        const std::size_t       numValues);
    void addInt32(
        const std::string&       name,
        std::unique_ptr<int32[]> value,
        const std::size_t        numValues);
    void addVector3R(
        const std::string&          name,
        std::unique_ptr<Vector3R[]> value,
        const std::size_t           numValues);
    void addString(
        const std::string&             name,
        std::unique_ptr<std::string[]> value,
        const std::size_t              numValues);

    bool     findBool(
        const std::string& name, const bool      defaultValue = false     );
    real     findReal (
        const std::string& name, const real      defaultValue = 0.0_r     );
    int32    findInt32(
        const std::string& name, const int32     defaultValue = 0         );
    Vector3R findVector3r(
        const std::string& name, const Vector3R& defaultValue = Vector3R());
    std::string findString(
        const std::string& name, const std::string& defaultValue = "");

    const std::vector<real>     findRealArray(
        const std::string& name);
    const std::vector<Vector3R> findVector3rArray(
        const std::string& name);

    std::shared_ptr<TTexture<real>> getRealTexture(
        const std::string&             name, 
        const TStringKeyMap<TTexture<real>>& realTextures,
        const real                          defaultValue = 0.0_r);
    std::shared_ptr<TTexture<Spectrum>> getSpectrumTexture(
        const std::string&                 name,
        const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures,
        const Spectrum&                         defaultValue = Spectrum(0.0_r));

    ESdClassType classType() const;

    void setClassType(const std::string& classType);
    void setClassType(const ESdClassType classType);

private:
    template<typename T>
    T _findData(
        const std::string& name, 
        const T& defaultValue,
        std::vector<std::shared_ptr<TSdDataUnit<T>>>& dataset);

    template<typename T>
    std::vector<T> _findDataArray(
        const std::string& name,
        std::vector<std::shared_ptr<TSdDataUnit<T>>>& dataset);

    ESdClassType _classType;

    std::vector<std::shared_ptr<TSdDataUnit<bool>>>        _bools;
    std::vector<std::shared_ptr<TSdDataUnit<real>>>        _reals;
    std::vector<std::shared_ptr<TSdDataUnit<int32>>>       _int32s;
    std::vector<std::shared_ptr<TSdDataUnit<Vector3R>>>    _vector3rs;
    std::vector<std::shared_ptr<TSdDataUnit<std::string>>> _strings;
};

// template header implementation

template<typename T>
inline T SdData::_findData(
    const std::string&                            name,
    const T&                                      defaultValue,
    std::vector<std::shared_ptr<TSdDataUnit<T>>>& dataset)
{
    for (std::size_t i = 0; i < dataset.size(); ++i) 
    {
        if (dataset[i]->name() == name && dataset[i]->numValues() == 1)
        {
            auto&& value = dataset[i]->value();
            dataset.erase(dataset.begin() + i);

            return value[0];
        }
    }

    return defaultValue;
}

template<typename T>
inline std::vector<T> SdData::_findDataArray(
    const std::string&                            name,
    std::vector<std::shared_ptr<TSdDataUnit<T>>>& dataset)
{
    std::vector<T> dataArray;
    for (std::size_t i = 0; i < dataset.size(); ++i)
    {
        if (dataset[i]->name() == name)
        {
            auto values = dataset[i]->value();
            dataset.erase(dataset.begin() + i);

            dataArray.reserve(values.size());
            for (std::size_t i = 0; i < values.size(); ++i)
            {
                dataArray.emplace_back(values[i]);
            }

            break;
        }
    }

    return std::move(dataArray);
}

} // namespace cadise