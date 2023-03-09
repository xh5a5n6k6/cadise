#include "file-io/scene-description/CSDResource.h"

#include "core/texture/category/tConstantTexture.h"
#include "math/tVector3.h"

namespace cadise
{

CSDResource::CSDResource() :
    _classType(ESdClassType::Undefined) 
{}

void CSDResource::addBool(const std::string& name, const std::string& valueRaw)
{
    _addRawData(name, valueRaw, _boolRaws);
}

void CSDResource::addString(const std::string& name, const std::string& valueRaw)
{
    _addRawData(name, valueRaw, _stringRaws);
}

void CSDResource::addInt(const std::string& name, const std::string& valueRaw)
{
    _addRawData(name, valueRaw, _intRaws);
}

void CSDResource::addFloat(const std::string& name, const std::string& valueRaw)
{
    _addRawData(name, valueRaw, _floatRaws);
}

void CSDResource::addVector2(const std::string& name, const std::vector<std::string>& valueRaw)
{
    _addRawData(name, valueRaw, _vector2Raws);
}

void CSDResource::addVector3(const std::string& name, const std::vector<std::string>& valueRaw)
{
    _addRawData(name, valueRaw, _vector3Raws);
}

void CSDResource::addFloatArray(const std::string& name, const std::vector<std::string>& valueRaw)
{
    _addRawData(name, valueRaw, _floatArrayRaws);
}

void CSDResource::addVector3Array(const std::string& name, const std::vector<std::string>& valueRaw)
{
    _addRawData(name, valueRaw, _vector3ArrayRaws);
}

bool CSDResource::findBool(
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

std::string CSDResource::findString(
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

std::shared_ptr<TTexture<real>> CSDResource::getRealTexture(
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

std::shared_ptr<TTexture<Spectrum>> CSDResource::getSpectrumTexture(
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

ESdClassType CSDResource::classType() const
{
    return _classType;
}

void CSDResource::setClassType(const std::string& classType) 
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

void CSDResource::setClassType(const ESdClassType classType)
{
    _classType = classType;
}

} // namespace cadise