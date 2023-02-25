#pragma once

#include "core/spectrum/spectrum.h"
#include "file-io/scene-description/eSdClassType.h"
#include "file-io/scene-description/valueParser.h"
#include "fundamental/assertion.h"
#include "math/type/mapType.h"
#include "math/tVector2.h"
#include "math/tVector3.h"

#include <memory>
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

    void addBool(const std::string& name, const std::string& valueRaw);
    void addString(const std::string& name, const std::string& valueRaw);
    void addInt(const std::string& name, const std::string& valueRaw);
    void addFloat(const std::string& name, const std::string& valueRaw);
    void addVector2(const std::string& name, const std::vector<std::string>& valueRaw);
    void addVector3(const std::string& name, const std::vector<std::string>& valueRaw);
    void addFloatArray(const std::string& name, const std::vector<std::string>& valueRaw);
    void addVector3Array(const std::string& name, const std::vector<std::string>& valueRaw);

    bool findBool(
        const std::string& name, 
        const bool         defaultValue = false);

    std::string findString(
        const std::string& name, 
        const std::string& defaultValue = "");

    template<typename IntType>
    IntType findInt(
        const std::string& name, 
        const IntType      defaultValue = static_cast<IntType>(0));

    template<typename FloatType>
    FloatType findFloat(
        const std::string& name,
        const FloatType    defaultValue = static_cast<FloatType>(0.0));

    template<typename ElementType>
    TVector2<ElementType> findVector2(
        const std::string&           name,
        const TVector2<ElementType>& defaultValue = TVector2<ElementType>(0));

    template<typename ElementType>
    TVector3<ElementType> findVector3(
        const std::string&           name,
        const TVector3<ElementType>& defaultValue = TVector3<ElementType>(0));

    template<typename FloatType>
    std::vector<FloatType> findFloatArray(const std::string& name);

    template<typename ElementType>
    std::vector<TVector3<ElementType>> findVector3Array(const std::string& name);

    std::shared_ptr<TTexture<real>> getRealTexture(
        const std::string&                   name, 
        const TStringKeyMap<TTexture<real>>& realTextures,
        const real                           defaultValue = 0.0_r);

    std::shared_ptr<TTexture<Spectrum>> getSpectrumTexture(
        const std::string&                       name,
        const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures,
        const Spectrum&                          defaultValue = Spectrum(0.0_r));

    ESdClassType classType() const;

    void setClassType(const std::string& classType);
    void setClassType(const ESdClassType classType);

private:
    template<typename RawDataType>
    void _addRawData(
        const std::string&                            name,
        const RawDataType&                            rawData,
        std::unordered_map<std::string, RawDataType>& rawDataMap);

    ESdClassType                                              _classType;
    std::unordered_map<std::string, std::string>              _boolRaws;
    std::unordered_map<std::string, std::string>              _intRaws;
    std::unordered_map<std::string, std::string>              _floatRaws;
    std::unordered_map<std::string, std::string>              _stringRaws;
    std::unordered_map<std::string, std::vector<std::string>> _vector2Raws;
    std::unordered_map<std::string, std::vector<std::string>> _vector3Raws;
    std::unordered_map<std::string, std::vector<std::string>> _floatArrayRaws;
    std::unordered_map<std::string, std::vector<std::string>> _vector3ArrayRaws;
};

// template header implementation

template<typename IntType>
inline IntType SdData::findInt(
    const std::string& name, 
    const IntType      defaultValue)
{
    if (_intRaws.contains(name))
    {
        return ValueParser::parseInt<IntType>(_intRaws[name]);
    }
    else
    {   
        return defaultValue;
    }
}

template<typename FloatType>
inline FloatType SdData::findFloat(
    const std::string& name,
    const FloatType    defaultValue)
{
    if (_floatRaws.contains(name))
    {
        return ValueParser::parseFloat<FloatType>(_floatRaws[name]);
    }
    else
    {
        return defaultValue;
    }
}

template<typename ElementType>
inline TVector2<ElementType> SdData::findVector2(
    const std::string&           name,
    const TVector2<ElementType>& defaultValue)
{
    if (_vector2Raws.contains(name))
    {
        return ValueParser::parseVector2<ElementType>(_vector2Raws[name]);
    }
    else
    {
        return defaultValue;
    }
}

template<typename ElementType>
inline TVector3<ElementType> SdData::findVector3(
    const std::string&           name,
    const TVector3<ElementType>& defaultValue)
{
    if (_vector3Raws.contains(name))
    {
        return ValueParser::parseVector3<ElementType>(_vector3Raws[name]);
    }
    else
    {
        return defaultValue;
    }
}

template<typename FloatType>
inline std::vector<FloatType> SdData::findFloatArray(const std::string& name)
{
    if (_floatArrayRaws.contains(name))
    {
        return ValueParser::parseFloatArray<FloatType>(_floatArrayRaws[name]);
    }
    else
    {
        return {};
    }
}

template<typename ElementType>
inline std::vector<TVector3<ElementType>> SdData::findVector3Array(const std::string& name)
{
    if (_vector3ArrayRaws.contains(name))
    {
        return ValueParser::parseVector3Array<ElementType>(_vector3ArrayRaws[name]);
    }
    else
    {
        return {};
    }
}

template<typename RawDataType>
inline void SdData::_addRawData(
    const std::string&                            name,
    const RawDataType&                            rawData,
    std::unordered_map<std::string, RawDataType>& rawDataMap)
{
    CS_ASSERT(!rawDataMap.contains(name));

    rawDataMap[name] = rawData;
}

} // namespace cadise