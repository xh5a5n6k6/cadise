#include "file-io/scene-description/sdData.h"

#include "core/texture/category/constantTexture.h"

namespace cadise {

SdData::SdData() :
    _classType(SdClassType::NONE) {
}

void SdData::addReal(
    const std::string_view& name, 
    std::unique_ptr<real[]> value, 
    const int32 valueNumber) {

    _reals.push_back(
        std::make_shared<SdDataUnit<real>>(name, std::move(value), valueNumber));
}

void SdData::addInt32(
    const std::string_view& name,
    std::unique_ptr<int32[]> value,
    const int32 valueNumber) {

    _int32s.push_back(
        std::make_shared<SdDataUnit<int32>>(name, std::move(value), valueNumber));
}

void SdData::addVector3R(
    const std::string_view& name,
    std::unique_ptr<Vector3R[]> value,
    const int32 valueNumber) {

    _vector3rs.push_back(
        std::make_shared<SdDataUnit<Vector3R>>(name, std::move(value), valueNumber));
}

void SdData::addString(
    const std::string_view& name,
    std::unique_ptr<std::string_view[]> value,
    const int32 valueNumber) {

    _strings.push_back(
        std::make_shared<SdDataUnit<std::string_view>>(name, std::move(value), valueNumber));
}

void SdData::addBsdfName(
    const std::string_view& name,
    std::unique_ptr<std::string_view[]> value,
    const int32 valueNumber) {

    _bsdfNames.push_back(
        std::make_shared<SdDataUnit<std::string_view>>(name, std::move(value), valueNumber));
}

void SdData::addTextureName(
    const std::string_view& name,
    std::unique_ptr<std::string_view[]> value,
    const int32 valueNumber) {

    _textureNames.push_back(
        std::make_shared<SdDataUnit<std::string_view>>(name, std::move(value), valueNumber));
}

real SdData::findReal(
    const std::string_view& name, const real& defaultValue) const {

    return _findData(name, defaultValue, _reals);
}

int32 SdData::findInt32(
    const std::string_view& name, const int32& defaultValue) const {

    return _findData(name, defaultValue, _int32s);
}

Vector3R SdData::findVector3r(
    const std::string_view& name, const Vector3R& defaultValue) const {

    return _findData(name, defaultValue, _vector3rs);
}

std::string_view SdData::findString(
    const std::string_view& name, const std::string_view& defaultValue) const {

    return _findData(name, defaultValue, _strings);
}

std::string_view SdData::findBsdfName(
    const std::string_view& name, const std::string_view& defaultValue) const {

    return _findData(name, defaultValue, _bsdfNames);
}

std::string_view SdData::findTextureName(
    const std::string_view& name, const std::string_view& defaultValue) const {

    return _findData(name, defaultValue, _textureNames);
}

const real* SdData::findRealArray(
    const std::string_view& name) const {

    return _findDataArray(name, _reals);
}

const Vector3R* SdData::findVector3rArray(
    const std::string_view& name) const {

    return _findDataArray(name, _vector3rs);
}

std::shared_ptr<Texture<real>> SdData::getRealTexture(
    const std::string_view& name,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures) const {

    std::shared_ptr<Texture<real>> realTexture = nullptr;

    const std::string_view textureName = findTextureName(name);
    if (textureName != "") {
        auto&& texture = realTextures.find(textureName);
        realTexture = texture->second;
    }
    else {
        const real value = findReal(name);
        realTexture = std::make_shared<ConstantTexture<real>>(value);
    }

    return realTexture;
}

std::shared_ptr<Texture<Spectrum>> SdData::getSpectrumTexture(
    const std::string_view& name,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) const {

    std::shared_ptr<Texture<Spectrum>> spectrumTexture = nullptr;

    const std::string_view textureName = findTextureName(name);
    if (textureName != "") {
        auto&& texture = spectrumTextures.find(textureName);
        spectrumTexture = texture->second;
    }
    else {
        // it now only support rgb spectrum
        const Vector3R rgb = findVector3r(name);
        spectrumTexture = std::make_shared<ConstantTexture<Spectrum>>(rgb);
    }

    return spectrumTexture;
}

SdClassType SdData::classType() const {
    return _classType;
}

void SdData::setClassType(const std::string_view& classType) {
    if (!classType.compare("camera")) {
        setClassType(SdClassType::CAMERA);
    }
    else if (!classType.compare("renderer")) {
        setClassType(SdClassType::RENDERER);
    }
    else if (!classType.compare("accelerator")) {
        setClassType(SdClassType::ACCELERATOR);
    }
    else if (!classType.compare("texture-real")) {
        setClassType(SdClassType::TEXTURE_REAL);
    }
    else if (!classType.compare("texture-spectrum")) {
        setClassType(SdClassType::TEXTURE_SPECTRUM);
    }
    else if (!classType.compare("material")) {
        setClassType(SdClassType::MATERIAL);
    }
    else if (!classType.compare("light")) {
        setClassType(SdClassType::LIGHT);
    }
    else if (!classType.compare("primitive")) {
        setClassType(SdClassType::PRIMITIVE);
    }
}

void SdData::setClassType(const SdClassType& classType) {
    _classType = classType;
}

} // namespace cadise