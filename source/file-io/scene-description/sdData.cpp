#include "file-io/scene-description/sdData.h"

#include "core/texture/category/constantTexture.h"

namespace cadise {

SdData::SdData() :
    _classType(ESdClassType::NONE) {
}

void SdData::addBool(
    const std::string_view& name, 
    std::unique_ptr<bool[]> value, 
    const int32 valueNumber) {

    _bools.push_back(
        std::make_shared<SdDataUnit<bool>>(name, std::move(value), valueNumber));
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

bool SdData::findBool(
    const std::string_view& name, const bool defaultValue) const {

    return _findData(name, defaultValue, _bools);
}

real SdData::findReal(
    const std::string_view& name, const real defaultValue) const {

    return _findData(name, defaultValue, _reals);
}

int32 SdData::findInt32(
    const std::string_view& name, const int32 defaultValue) const {

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

const std::vector<real> SdData::findRealArray(
    const std::string_view& name) const {

    return _findDataArray(name, _reals);
}

const std::vector<Vector3R> SdData::findVector3rArray(
    const std::string_view& name) const {

    return _findDataArray(name, _vector3rs);
}

std::shared_ptr<Texture<real>> SdData::getRealTexture(
    const std::string_view&            name,
    const StringKeyMap<Texture<real>>& realTextures,
    const real                         defaultValue) const {

    std::shared_ptr<Texture<real>> realTexture = nullptr;

    const std::string_view textureName = this->findString(name);
    if (textureName != "") {
        auto&& texture = realTextures.find(textureName);
        realTexture = texture->second;
    }
    else {
        const real value = this->findReal(name, defaultValue);
        realTexture = std::make_shared<ConstantTexture<real>>(value);
    }

    return realTexture;
}

std::shared_ptr<Texture<Spectrum>> SdData::getSpectrumTexture(
    const std::string_view&                name,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures,
    const Spectrum&                        defaultValue) const {

    std::shared_ptr<Texture<Spectrum>> spectrumTexture = nullptr;

    const std::string_view textureName = this->findString(name);
    if (textureName != "") {
        auto&& texture = spectrumTextures.find(textureName);
        spectrumTexture = texture->second;
    }
    else {
        // it now only support rgb spectrum
        Vector3R defaultRgb;
        defaultValue.transformToRgb(&defaultRgb);

        const Vector3R rgb = this->findVector3r(name, defaultRgb);
        spectrumTexture = std::make_shared<ConstantTexture<Spectrum>>(Spectrum(rgb));
    }

    return spectrumTexture;
}

const ESdClassType& SdData::classType() const {
    return _classType;
}

void SdData::setClassType(const std::string_view& classType) {
    if (classType == "film") {
        setClassType(ESdClassType::FILM);
    }
    else if (classType == "camera") {
        setClassType(ESdClassType::CAMERA);
    }
    else if (classType == "renderer") {
        setClassType(ESdClassType::RENDERER);
    }
    else if (classType == "accelerator") {
        setClassType(ESdClassType::ACCELERATOR);
    }
    else if (classType == "light-cluster") {
        setClassType(ESdClassType::LIGHT_CLUSTER);
    }
    else if (classType == "texture-real") {
        setClassType(ESdClassType::TEXTURE_REAL);
    }
    else if (classType == "texture-spectrum") {
        setClassType(ESdClassType::TEXTURE_SPECTRUM);
    }
    else if (classType == "material") {
        setClassType(ESdClassType::MATERIAL);
    }
    else if (classType == "light") {
        setClassType(ESdClassType::LIGHT);
    }
    else if (classType == "primitive") {
        setClassType(ESdClassType::PRIMITIVE);
    }
    else {
        // unsupported type
    }
}

void SdData::setClassType(const ESdClassType& classType) {
    _classType = classType;
}

} // namespace cadise