#include "FileIO/StringUtility.h"

#include <unordered_set>

namespace cadise
{

static const std::unordered_set<char> regexReservedChars =
{
    '+',
    '*',
    '?',
    '^',
    '$',
    '(',
    ')',
    '[',
    ']',
    '{',
    '}',
    '|',
    '\\',
};

std::string StringUtility::trimHead(
    const std::string& source,
    const char         trimChar)
{
    std::string result(source);
    trimHeadLocal(result, trimChar);

    return result;
}

std::string StringUtility::trimTail(
    const std::string& source,
    const char         trimChar)
{
    std::string result(source);
    trimTailLocal(result, trimChar);

    return result;
}

std::string StringUtility::trim(
    const std::string& source,
    const char         trimChar)
{
    std::string result(source);
    trimLocal(result, trimChar);

    return result;
}

std::string& StringUtility::trimHeadLocal(
    std::string& source,
    const char   trimChar)
{
    // Remove chars from the beginning with specified length.
    source.erase(0, source.find_first_not_of(trimChar));

    return source;
}

std::string& StringUtility::trimTailLocal(
    std::string& source,
    const char   trimChar)
{
    // Remove all chars began from specified index (including).
    source.erase(source.find_last_not_of(trimChar) + 1);

    return source;
}

std::string& StringUtility::trimLocal(
    std::string& source,
    const char   trimChar)
{
    return trimHeadLocal(trimTailLocal(source, trimChar), trimChar);
}

std::string StringUtility::escapeRegexChar(const char regexChar)
{
    std::string result = "";
    if (regexReservedChars.contains(regexChar))
    {
        result += "\\";
    }

    result += regexChar;

    return result;
}

} // namespace cadise