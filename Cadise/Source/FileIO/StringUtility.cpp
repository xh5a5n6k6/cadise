#include "file-io/string_utils.h"

#include <unordered_set>

namespace cadise::string_utils
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

std::string trim_head(
    const std::string& source,
    const char         trimChar)
{
    std::string result(source);
    trim_head_local(result, trimChar);

    return result;
}

std::string trim_tail(
    const std::string& source,
    const char         trimChar)
{
    std::string result(source);
    trim_tail_local(result, trimChar);

    return result;
}

std::string trim(
    const std::string& source,
    const char         trimChar)
{
    std::string result(source);
    trim_local(result, trimChar);

    return result;
}

std::string& trim_head_local(
    std::string& source,
    const char   trimChar)
{
    // Remove chars from the beginning with specified length.
    source.erase(0, source.find_first_not_of(trimChar));

    return source;
}

std::string& trim_tail_local(
    std::string& source,
    const char   trimChar)
{
    // Remove all chars began from specified index (including).
    source.erase(source.find_last_not_of(trimChar) + 1);

    return source;
}

std::string& trim_local(
    std::string& source,
    const char   trimChar)
{
    return trim_head_local(trim_tail_local(source, trimChar), trimChar);
}

std::string escape_regex_char(const char regexChar)
{
    std::string result = "";
    if (regexReservedChars.contains(regexChar))
    {
        result += "\\";
    }

    result += regexChar;

    return result;
}

} // namespace cadise::string_utils