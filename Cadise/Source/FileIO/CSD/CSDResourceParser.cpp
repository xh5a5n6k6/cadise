#include "FileIO/CSD/CSDResourceParser.h"

#include "FileIO/CSD/CSDResource.h"
#include "FileIO/StringUtility.h"
#include "FileIO/Tokenizer.h"
#include "Foundation/Logging/Logger.h"

#include <string>

namespace cadise::internal
{

std::vector<std::string> tokenizeValueRaw(const std::string& valueRaw)
{
    static const Tokenizer tokenizer = Tokenizer::makeFromDelimiters({ ',', ' ' });

    std::vector<std::string> tokens;
    tokenizer.tokenize(valueRaw, tokens);

    return tokens;
}

} // namespace cadise::internal

namespace cadise
{

namespace
{
    const Logger logger("CSDResourceParser");
}

CSDResourceParser::CSDResourceParser() = default;

std::shared_ptr<CSDResource> CSDResourceParser::parse(const std::string& dataRaw) const
{
    std::string sdClassType;
    std::string sdDataRaw;
    {
        const Tokenizer tokenizer = Tokenizer::makeFromDelimiters({ '{', '}' });

        std::vector<std::string> nameValuePair;
        tokenizer.tokenize(dataRaw, nameValuePair);
        if (nameValuePair.size() != 2)
        {
            logger.log("Invalid sd data, skip parsing processing!");
            return std::make_shared<CSDResource>();
        }

        // TODO: support multiple trimmed elements
        StringUtility::trimLocal(nameValuePair[0], ' ');
        StringUtility::trimLocal(nameValuePair[0], '\n');

        sdClassType = nameValuePair[0];
        sdDataRaw   = nameValuePair[1];
    }

    CSDResource sdData;
    sdData.setClassType(sdClassType);

    {
        // tokenize each SdDataUnit, which is string enclosed by '[' and ']'
        const Tokenizer tokenizer = Tokenizer::makeFromOpenClosePattern('[', ']', false);

        std::vector<std::string> dataUnitRaws;
        tokenizer.captureMatching(sdDataRaw, dataUnitRaws);

        // dataUnitRaw doesn't include '[' and ']'
        for (const auto& dataUnitRaw : dataUnitRaws)
        {
            _parseDataUnit(dataUnitRaw, &sdData);
        }
    }

    return std::make_shared<CSDResource>(std::move(sdData));
}

void CSDResourceParser::_parseDataUnit(
    const std::string& dataUnitString,
    CSDResource* const out_sdData) const
{
    // TODO: redesign syntax of data unit

    // HACK: extract raw value enclosed by '"' and '"'
    std::string valueRaw;
    {
        const Tokenizer valueTokenizer = Tokenizer::makeFromOpenClosePattern(
            '\"',
            '\"',
            false);

        std::vector<std::string> tokens;
        valueTokenizer.captureMatching(dataUnitString, tokens);

        if (tokens.size() != 1)
        {
            logger.log("Can't find valid matching! Set value to empty string.");
            valueRaw = "";
        }
        else
        {
            valueRaw = tokens[0];
        }
    }

    std::string type;
    std::string name;
    {
        const Tokenizer tokenizer = Tokenizer::makeFromDelimiters({ ' ' });

        std::vector<std::string> tokens;
        tokenizer.tokenize(dataUnitString, tokens);

        if (tokens.size() < 2)
        {
            logger.log(ELogLevel::Warn, "Detect syntax error on data unit, skip parsing!");
            return;
        }

        name = tokens[0];
        type = tokens[1];
    }

    if (type == "bool")
    {
        out_sdData->addBool(name, valueRaw);
    }
    else if (type == "real")
    {
        out_sdData->addFloat(name, valueRaw);
    }
    else if (type == "integer")
    {
        out_sdData->addInt(name, valueRaw);
    }
    else if (
        type == "string" ||
        type == "material" ||
        type == "texture" ||
        type == "primitive")
    {
        out_sdData->addString(name, valueRaw);
    }
    else if (type == "vector2")
    {
        const auto splitValuesRaw = internal::tokenizeValueRaw(valueRaw);
        out_sdData->addVector2(name, splitValuesRaw);
    }
    else if (
        type == "vector3" ||
        type == "rgb")
    {
        const auto splitValuesRaw = internal::tokenizeValueRaw(valueRaw);
        out_sdData->addVector3(name, splitValuesRaw);
    }
    else if (type == "real-array")
    {
        const auto splitValuesRaw = internal::tokenizeValueRaw(valueRaw);
        out_sdData->addFloatArray(name, splitValuesRaw);
    }
    else if (type == "vector3-array")
    {
        const auto splitValuesRaw = internal::tokenizeValueRaw(valueRaw);
        out_sdData->addVector3Array(name, splitValuesRaw);
    }
}

} // namespace cadise