#include "file-io/scene-description/sdData.h"

#include "core/texture/category/tConstantTexture.h"
#include "math/tVector3.h"

namespace cadise
{

SdData::SdData() :
    _classType(ESdClassType::Undefined) 
{}

void SdData::addBool(const std::string& name, const std::string& valueRaw)
{
    _addRawData(name, valueRaw, _boolRaws);
}

void SdData::addString(const std::string& name, const std::string& valueRaw)
{
    _addRawData(name, valueRaw, _stringRaws);
}

void SdData::addInt(const std::string& name, const std::string& valueRaw)
{
    _addRawData(name, valueRaw, _intRaws);
}

void SdData::addFloat(const std::string& name, const std::string& valueRaw)
{
    _addRawData(name, valueRaw, _floatRaws);
}

void SdData::addVector2(const std::string& name, const std::vector<std::string>& valueRaw)
{
    _addRawData(name, valueRaw, _vector2Raws);
}

void SdData::addVector3(const std::string& name, const std::vector<std::string>& valueRaw)
{
    _addRawData(name, valueRaw, _vector3Raws);
}

void SdData::addFloatArray(const std::string& name, const std::vector<std::string>& valueRaw)
{
    _addRawData(name, valueRaw, _floatArrayRaws);
}

void SdData::addVector3Array(const std::string& name, const std::vector<std::string>& valueRaw)
{
    _addRawData(name, valueRaw, _vector3ArrayRaws);
}

bool SdData::findBool(
    const std::string& name, 
    const bool         defaultValue)
{
    if (_boolRaws.contains(name))
    {
        return ValueParser::parseBool(_boolRaws[name]);
    }
    else
    {
        return defaultValue;
    }
}

std::string SdData::findString(
    const std::string& name, 
    const std::string& defaultValue)
{
    if (_stringRaws.contains(name))
    {
        return ValueParser::parseString(_stringRaws[name]);
    }
    else
    {
        return defaultValue;
    }
}

std::shared_ptr<TTexture<real>> SdData::getRealTexture(
    const std::string&                   name,
    const TStringKeyMap<TTexture<real>>& realTextures,
    const real                           defaultValue)
{
    std::shared_ptr<TTexture<real>> realTexture;

    const std::string textureName = this->findString(name);
    if (!textureName.empty()) 
    {
        realTexture = realTextures.at(textureName);
    }
    else 
    {
        const real value = this->findFloat<real>(name, defaultValue);
        realTexture = std::make_shared<TConstantTexture<real>>(value);
    }

    return realTexture;
}

std::shared_ptr<TTexture<Spectrum>> SdData::getSpectrumTexture(
    const std::string&                       name,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures,
    const Spectrum&                          defaultValue)
{
    std::shared_ptr<TTexture<Spectrum>> spectrumTexture;

    const std::string textureName = this->findString(name);
    if (!textureName.empty()) 
    {
        spectrumTexture = spectrumTextures.at(textureName);
    }
    else 
    {
        // it now only support rgb spectrum
        Vector3R defaultLinearSrgb;
        defaultValue.transformToLinearSrgb(&defaultLinearSrgb);

        const Vector3R linearSrgb = this->findVector3<real>(name, defaultLinearSrgb);
        spectrumTexture = std::make_shared<TConstantTexture<Spectrum>>(Spectrum(linearSrgb));
    }

    return spectrumTexture;
}

ESdClassType SdData::classType() const
{
    return _classType;
}

void SdData::setClassType(const std::string& classType) 
{
    if (classType == "film") 
    {
        this->setClassType(ESdClassType::Film);
    }
    else if (classType == "camera")
    {
        this->setClassType(ESdClassType::Camera);
    }
    else if (classType == "renderer")
    {
        this->setClassType(ESdClassType::Renderer);
    }
    else if (classType == "accelerator") 
    {
        this->setClassType(ESdClassType::Accelerator);
    }
    else if (classType == "light-cluster") 
    {
        this->setClassType(ESdClassType::LightCluster);
    }
    else if (classType == "texture-real") 
    {
        this->setClassType(ESdClassType::TextureReal);
    }
    else if (classType == "texture-spectrum")
    {
        this->setClassType(ESdClassType::TextureSpectrum);
    }
    else if (classType == "material") 
    {
        this->setClassType(ESdClassType::Material);
    }
    else if (classType == "light") 
    {
        this->setClassType(ESdClassType::Light);
    }
    else if (classType == "primitive") 
    {
        this->setClassType(ESdClassType::Primitive);
    }
    else 
    {
        // unsupported type
    }
}

void SdData::setClassType(const ESdClassType classType)
{
    _classType = classType;
}

} // namespace cadise