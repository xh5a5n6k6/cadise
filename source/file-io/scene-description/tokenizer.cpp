#include "file-io/scene-description/tokenizer.h"

#include "file-io/scene-description/sdData.h"

#include <cctype>
#include <string>

namespace cadise {

Tokenizer::Tokenizer() = default;

std::shared_ptr<SdData> Tokenizer::tokenize(const std::string_view& data) const {
    std::shared_ptr<SdData> sdData = std::make_shared<SdData>();

    uint64 startPosition = 0;
    uint64 endPosition   = 0;

    // tokenize sdClassType, which is string before '{'
    while (!std::isalpha(data[startPosition])) {
        startPosition += 1;
    }
    endPosition = data.find_first_of('{', startPosition);
    while (!std::isalpha(data[endPosition])) {
        endPosition -= 1;
    }
    std::string_view classType = data.substr(startPosition, endPosition - startPosition + 1);
    sdData->setClassType(classType);

    // tokenize each SdDataUnit, which is string enclosed by '[' and ']'
    while ((startPosition = data.find_first_of('[', endPosition)) != std::string_view::npos) {
        endPosition = data.find_first_of(']', startPosition);
        uint64 dataUnitLength = endPosition - startPosition - 1;

        // dataUnitString doesn't include '[' and ']'
        std::string_view dataUnitString = data.substr(startPosition + 1, dataUnitLength);
        _parseDataUnit(sdData, dataUnitString);

        startPosition = endPosition + 1;
    }

    return sdData;
}

void Tokenizer::_parseDataUnit(std::shared_ptr<SdData>& sdData, const std::string_view& dataUnitString) const {
    uint64 startPosition = 0;
    uint64 endPosition   = 0;
    
    // parse variable name
    startPosition = dataUnitString.find_first_not_of(' ', endPosition);
    endPosition   = dataUnitString.find_first_of(' ', startPosition);
    std::string_view name = dataUnitString.substr(startPosition, endPosition - startPosition);

    // parse variable type
    startPosition = dataUnitString.find_first_not_of(' ', endPosition + 1);
    endPosition   = dataUnitString.find_first_of(' ', startPosition);
    std::string_view type = dataUnitString.substr(startPosition, endPosition - startPosition);

    // parse variable value
    startPosition = dataUnitString.find_first_of('\"', endPosition);
    endPosition   = dataUnitString.find_last_of('\"');
    std::string_view valueString = dataUnitString.substr(startPosition + 1, endPosition - startPosition - 1);

    // parse value string to corresponding type
    if (!type.compare("bool")) {
        std::unique_ptr<bool[]> value = std::move(_parseBool(valueString));
        sdData->addBool(name, std::move(value), 1);
    }
    else if (!type.compare("real")) {
        std::unique_ptr<real[]> value = std::move(_parseReal(valueString));
        sdData->addReal(name, std::move(value), 1);
    }
    else if (!type.compare("int32")) {
        std::unique_ptr<int32[]> value = std::move(_parseInt32(valueString));
        sdData->addInt32(name, std::move(value), 1);
    }
    else if (!type.compare("rgb")) {
        std::unique_ptr<Vector3R[]> value = std::move(_parseVector3R(valueString));
        sdData->addVector3R(name, std::move(value), 1);
    }
    else if (!type.compare("vector3r")) {
        std::unique_ptr<Vector3R[]> value = std::move(_parseVector3R(valueString));
        sdData->addVector3R(name, std::move(value), 1);
    }
    else if (!type.compare("string")) {
        std::unique_ptr<std::string_view[]> value = std::move(_parseString(valueString));
        sdData->addString(name, std::move(value), 1);
    }
    else if (!type.compare("material")) {
        std::unique_ptr<std::string_view[]> value = std::move(_parseString(valueString));
        sdData->addString(name, std::move(value), 1);
    }
    else if (!type.compare("texture")) {
        std::unique_ptr<std::string_view[]> value = std::move(_parseString(valueString));
        sdData->addString(name, std::move(value), 1);
    }
    else if (!type.compare("primitive")) {
        std::unique_ptr<std::string_view[]> value = std::move(_parseString(valueString));
        sdData->addString(name, std::move(value), 1);
    }
    else if (!type.compare("real-array")) {
        int32 valueNumber = 0;
        std::unique_ptr<real[]> value = std::move(_parseRealArray(valueString, &valueNumber));
        sdData->addReal(name, std::move(value), valueNumber);
    }
    else if (!type.compare("vector3r-array")) {
        int32 valueNumber = 0;
        std::unique_ptr<Vector3R[]> value = std::move(_parseVector3RArray(valueString, &valueNumber));
        sdData->addVector3R(name, std::move(value), valueNumber);
    }
    else {
        // unknown type
    }
}

std::unique_ptr<bool[]> Tokenizer::_parseBool(const std::string_view& value) const {
    std::unique_ptr<bool[]> result(new bool);

    if (!value.compare("true")) {
        result[0] = true;
    }
    else if (!value.compare("false")) {
        result[0] = false;
    }
    else {
        // go wrong
    }

    return std::move(result);
}

std::unique_ptr<real[]> Tokenizer::_parseReal(const std::string_view& value) const {
    std::unique_ptr<real[]> result(new real);

    real actualValue = static_cast<real>(std::stold(std::string(value)));
    result[0] = actualValue;

    return std::move(result);
}

std::unique_ptr<int32[]> Tokenizer::_parseInt32(const std::string_view& value) const {
    std::unique_ptr<int32[]> result(new int32);

    int32 actualValue = static_cast<int32>(std::stoll(std::string(value)));
    result[0] = actualValue;

    return std::move(result);
}

std::unique_ptr<Vector3R[]> Tokenizer::_parseVector3R(const std::string_view& value) const {
    std::unique_ptr<Vector3R[]> result(new Vector3R);

    uint64 startPosition = 0;
    uint64 endPosition   = 0;
    std::string x;
    std::string y;
    std::string z;

    startPosition = value.find_first_not_of(' ', endPosition);
    endPosition   = value.find_first_of(' ', startPosition);
    x = value.substr(startPosition, endPosition - startPosition);

    startPosition = value.find_first_not_of(' ', endPosition + 1);
    endPosition   = value.find_first_of(' ', startPosition);
    y = value.substr(startPosition, endPosition - startPosition);

    startPosition = value.find_first_not_of(' ', endPosition + 1);
    endPosition   = value.find_last_not_of(' ');
    z = value.substr(startPosition, endPosition - startPosition + 1);

    result[0] = Vector3R(static_cast<real>(std::stold(x)),
                         static_cast<real>(std::stold(y)),
                         static_cast<real>(std::stold(z)));

    return std::move(result);
}

std::unique_ptr<std::string_view[]> Tokenizer::_parseString(const std::string_view& value) const {
    std::unique_ptr<std::string_view[]> result(new std::string_view);

    result[0] = value;

    return std::move(result);
}

std::unique_ptr<real[]> Tokenizer::_parseRealArray(
    const std::string_view& value, int32* const valueNumber) const {

    std::vector<real> realVector;
    // while real loop

    std::unique_ptr<real[]> result(new real[realVector.size()]);
    for (uint64 index = 0; index < realVector.size(); index++) {
        result[index] = realVector[index];
    }

    *valueNumber = static_cast<int32>(realVector.size());
    return std::move(result);
}

std::unique_ptr<Vector3R[]> Tokenizer::_parseVector3RArray(
    const std::string_view& value, int32* const valueNumber) const {

    std::vector<Vector3R> vector3RVector;

    uint64 startPosition = 0;
    uint64 endPosition   = 0;

    // while vector3r loop
    while ((startPosition = value.find_first_not_of(' ', startPosition)) != std::string_view::npos) {
        endPosition = value.find_first_of(',', startPosition);
        if (endPosition == std::string_view::npos) {
            endPosition = value.find_last_not_of(' ') + 1;
        }

        std::string x;
        std::string y;
        std::string z;

        uint64 tmpPosition = value.find_first_of(' ', startPosition);
        x = value.substr(startPosition, tmpPosition - startPosition);

        startPosition = value.find_first_not_of(' ', tmpPosition + 1);
        tmpPosition = value.find_first_of(' ', startPosition);
        y = value.substr(startPosition, tmpPosition - startPosition);

        startPosition = value.find_first_not_of(' ', tmpPosition + 1);
        z = value.substr(startPosition, endPosition - startPosition);

        vector3RVector.push_back(Vector3R(static_cast<real>(std::stold(x)),
                                          static_cast<real>(std::stold(y)),
                                          static_cast<real>(std::stold(z))));

        startPosition = endPosition + 1;
    }

    std::unique_ptr<Vector3R[]> result(new Vector3R[vector3RVector.size()]);
    for (uint64 index = 0; index < vector3RVector.size(); index++) {
        result[index] = vector3RVector[index];
    }

    *valueNumber = static_cast<int32>(vector3RVector.size());
    return std::move(result);
}

} // namespace cadise