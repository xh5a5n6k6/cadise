#include "file-io/scene-description/sdDataParser.h"

#include "file-io/scene-description/sdData.h"

#include <cctype>
#include <string>

namespace cadise
{

SdDataParser::SdDataParser() = default;

std::shared_ptr<SdData> SdDataParser::parse(const std::string& data) const
{
    std::shared_ptr<SdData> sdData = std::make_shared<SdData>();

    std::size_t startPosition = 0;
    std::size_t endPosition   = 0;

    // tokenize sdClassType, which is string before '{'
    while (!std::isalpha(data[startPosition])) 
    {
        startPosition += 1;
    }
    endPosition = data.find_first_of('{', startPosition);
    while (!std::isalpha(data[endPosition])) 
    {
        endPosition -= 1;
    }
    
    const std::string classType = data.substr(startPosition, endPosition - startPosition + 1);
    sdData->setClassType(classType);

    // tokenize each SdDataUnit, which is string enclosed by '[' and ']'
    while ((startPosition = data.find_first_of('[', endPosition)) != std::string::npos)
    {
        endPosition = data.find_first_of(']', startPosition);
        const std::size_t dataUnitLength = endPosition - startPosition - 1;

        // dataUnitString doesn't include '[' and ']'
        const std::string dataUnitString = data.substr(startPosition + 1, dataUnitLength);
        _parseDataUnit(sdData, dataUnitString);

        startPosition = endPosition + 1;
    }

    return sdData;
}

void SdDataParser::_parseDataUnit(std::shared_ptr<SdData>& sdData, const std::string& dataUnitString) const
{
    std::size_t startPosition = 0;
    std::size_t endPosition   = 0;
    
    // parse variable name
    startPosition = dataUnitString.find_first_not_of(' ', endPosition);
    endPosition   = dataUnitString.find_first_of(' ', startPosition);
    const std::string name = dataUnitString.substr(startPosition, endPosition - startPosition);

    // parse variable type
    startPosition = dataUnitString.find_first_not_of(' ', endPosition + 1);
    endPosition   = dataUnitString.find_first_of(' ', startPosition);
    const std::string type = dataUnitString.substr(startPosition, endPosition - startPosition);

    // parse variable value
    startPosition = dataUnitString.find_first_of('\"', endPosition);
    endPosition   = dataUnitString.find_last_of('\"');
    const std::string valueString = dataUnitString.substr(startPosition + 1, endPosition - startPosition - 1);

    // parse value string to corresponding type
    if (type == "bool") 
    {
        std::unique_ptr<bool[]> value = std::move(_parseBool(valueString));
        sdData->addBool(name, std::move(value), 1);
    }
    else if (type == "real") 
    {
        std::unique_ptr<real[]> value = std::move(_parseReal(valueString));
        sdData->addReal(name, std::move(value), 1);
    }
    else if (type == "int32")
    {
        std::unique_ptr<int32[]> value = std::move(_parseInt32(valueString));
        sdData->addInt32(name, std::move(value), 1);
    }
    else if (type == "rgb")
    {
        std::unique_ptr<Vector3R[]> value = std::move(_parseVector3r(valueString));
        sdData->addVector3R(name, std::move(value), 1);
    }
    else if (type == "vector3r")
    {
        std::unique_ptr<Vector3R[]> value = std::move(_parseVector3r(valueString));
        sdData->addVector3R(name, std::move(value), 1);
    }
    else if (type == "string")
    {
        std::unique_ptr<std::string[]> value = std::move(_parseString(valueString));
        sdData->addString(name, std::move(value), 1);
    }
    else if (type == "material") 
    {
        std::unique_ptr<std::string[]> value = std::move(_parseString(valueString));
        sdData->addString(name, std::move(value), 1);
    }
    else if (type == "texture") 
    {
        std::unique_ptr<std::string[]> value = std::move(_parseString(valueString));
        sdData->addString(name, std::move(value), 1);
    }
    else if (type == "primitive") 
    {
        std::unique_ptr<std::string[]> value = std::move(_parseString(valueString));
        //sdData->addString(name, std::move(value), 1);
        sdData->addString(name, std::move(value), 1);
    }
    else if (type == "real-array") 
    {
        int32 valueNumber = 0;
        std::unique_ptr<real[]> value = std::move(_parseRealArray(valueString, &valueNumber));
        sdData->addReal(name, std::move(value), valueNumber);
    }
    else if (type == "vector3r-array") 
    {
        int32 valueNumber = 0;
        std::unique_ptr<Vector3R[]> value = std::move(_parseVector3rArray(valueString, &valueNumber));
        sdData->addVector3R(name, std::move(value), valueNumber);
    }
    else
    {
        // unknown type
        std::cout << "Sd data unit unknown type";
    }
}

std::unique_ptr<bool[]> SdDataParser::_parseBool(const std::string& value) const
{
    std::unique_ptr<bool[]> result(new bool[1]);

    if (value == "true") 
    {
        result[0] = true;
    }
    else if (value == "false") 
    {
        result[0] = false;
    }
    else 
    {
        // go wrong
    }

    return std::move(result);
}

std::unique_ptr<real[]> SdDataParser::_parseReal(const std::string& value) const 
{
    std::unique_ptr<real[]> result(new real[1]);

    const real actualValue = static_cast<real>(std::stold(value));
    result[0] = actualValue;

    return std::move(result);
}

std::unique_ptr<int32[]> SdDataParser::_parseInt32(const std::string& value) const 
{
    std::unique_ptr<int32[]> result(new int32[1]);

    const int32 actualValue = static_cast<int32>(std::stoll(value));
    result[0] = actualValue;

    return std::move(result);
}

std::unique_ptr<Vector3R[]> SdDataParser::_parseVector3r(const std::string& value) const 
{
    std::unique_ptr<Vector3R[]> result(new Vector3R[1]);

    std::size_t startPosition = 0;
    std::size_t endPosition   = 0;
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

std::unique_ptr<std::string[]> SdDataParser::_parseString(const std::string& value) const
{
    std::unique_ptr<std::string[]> result(new std::string[1]);

    result[0] = std::string(value);

    return std::move(result);
}

std::unique_ptr<real[]> SdDataParser::_parseRealArray(
    const std::string& value, 
    int32* const       out_valueNumber) const 
{
    std::vector<real> realVector;
    std::size_t startPosition = 0;
    std::size_t endPosition   = 0;

    // TODO : while real loop

    std::unique_ptr<real[]> result(new real[realVector.size()]);
    for (std::size_t i = 0; i < realVector.size(); ++i) 
    {
        result[i] = realVector[i];
    }

    *out_valueNumber = static_cast<int32>(realVector.size());
    return std::move(result);
}

std::unique_ptr<Vector3R[]> SdDataParser::_parseVector3rArray(
    const std::string& value, 
    int32* const       out_valueNumber) const
{
    std::vector<Vector3R> vector3rVector;
    std::size_t startPosition = 0;
    std::size_t endPosition   = 0;

    // while vector3r loop
    while ((startPosition = value.find_first_not_of(' ', startPosition)) != std::string::npos)
    {
        endPosition = value.find_first_of(',', startPosition);
        if (endPosition == std::string::npos) 
        {
            endPosition = value.find_last_not_of(' ') + 1;
        }

        std::string x;
        std::string y;
        std::string z;

        std::size_t tmpPosition = value.find_first_of(' ', startPosition);
        x = value.substr(startPosition, tmpPosition - startPosition);

        startPosition = value.find_first_not_of(' ', tmpPosition + 1);
        tmpPosition = value.find_first_of(' ', startPosition);
        y = value.substr(startPosition, tmpPosition - startPosition);

        startPosition = value.find_first_not_of(' ', tmpPosition + 1);
        z = value.substr(startPosition, endPosition - startPosition);

        vector3rVector.push_back(Vector3R(static_cast<real>(std::stold(x)),
                                          static_cast<real>(std::stold(y)),
                                          static_cast<real>(std::stold(z))));

        startPosition = endPosition + 1;
    }

    std::unique_ptr<Vector3R[]> result(new Vector3R[vector3rVector.size()]);
    for (std::size_t i = 0; i < vector3rVector.size(); ++i)
    {
        result[i] = vector3rVector[i];
    }

    *out_valueNumber = static_cast<int32>(vector3rVector.size());
    return std::move(result);
}

} // namespace cadise