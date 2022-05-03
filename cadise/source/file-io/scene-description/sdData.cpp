#include "file-io/scene-description/sdData.h"

#include "core/texture/category/tConstantTexture.h"

namespace cadise
{

SdData::SdData() :
    _classType(ESdClassType::Undefined) 
{}

void SdData::addBool(
    const std::string& name, 
    std::unique_ptr<bool[]> value, 
    const std::size_t numValues)
{
    _bools.emplace_back(
        std::make_shared<TSdDataUnit<bool>>(name, std::move(value), numValues));
}

void SdData::addReal(
    const std::string& name, 
    std::unique_ptr<real[]> value, 
    const std::size_t numValues)
{
    _reals.emplace_back(
        std::make_shared<TSdDataUnit<real>>(name, std::move(value), numValues));
}

void SdData::addInt32(
    const std::string& name,
    std::unique_ptr<int32[]> value,
    const std::size_t numValues)
{
    _int32s.emplace_back(
        std::make_shared<TSdDataUnit<int32>>(name, std::move(value), numValues));
}

void SdData::addVector3R(
    const std::string& name,
    std::unique_ptr<Vector3R[]> value,
    const std::size_t numValues)
{
    _vector3rs.emplace_back(
        std::make_shared<TSdDataUnit<Vector3R>>(name, std::move(value), numValues));
}

void SdData::addString(
    const std::string& name,
    std::unique_ptr<std::string[]> value,
    const std::size_t numValues)
{
    _strings.emplace_back(
        std::make_shared<TSdDataUnit<std::string>>(name, std::move(value), numValues));
}

bool SdData::findBool(
    const std::string& name, const bool defaultValue)
{
    return _findData(name, defaultValue, _bools);
}

real SdData::findReal(
    const std::string& name, const real defaultValue)
{
    return _findData(name, defaultValue, _reals);
}

int32 SdData::findInt32(
    const std::string& name, const int32 defaultValue)
{
    return _findData(name, defaultValue, _int32s);
}

Vector3R SdData::findVector3r(
    const std::string& name, const Vector3R& defaultValue)
{
    return _findData(name, defaultValue, _vector3rs);
}

std::string SdData::findString(
    const std::string& name, const std::string& defaultValue)
{
    return _findData(name, defaultValue, _strings);
}

const std::vector<real> SdData::findRealArray(
    const std::string& name)
{
    return _findDataArray(name, _reals);
}

const std::vector<Vector3R> SdData::findVector3rArray(
    const std::string& name)
{
    return _findDataArray(name, _vector3rs);
}

std::shared_ptr<TTexture<real>> SdData::getRealTexture(
    const std::string&             name,
    const TStringKeyMap<TTexture<real>>& realTextures,
    const real                          defaultValue)
{
    std::shared_ptr<TTexture<real>> realTexture = nullptr;

    const std::string textureName = this->findString(name);
    if (!textureName.empty()) 
    {
        realTexture = realTextures.at(textureName);
    }
    else 
    {
        const real value = this->findReal(name, defaultValue);
        realTexture = std::make_shared<TConstantTexture<real>>(value);
    }

    return realTexture;
}

std::shared_ptr<TTexture<Spectrum>> SdData::getSpectrumTexture(
    const std::string&                 name,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures,
    const Spectrum&                         defaultValue)
{
    std::shared_ptr<TTexture<Spectrum>> spectrumTexture = nullptr;

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

        const Vector3R linearSrgb = this->findVector3r(name, defaultLinearSrgb);
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