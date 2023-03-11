#pragma once

#include <string>

namespace cadise
{

/*
    Some string utilities

    - string trimming
    Reference: https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
*/
class StringUtility
{
public:
    static std::string trimHead(
        const std::string& source,
        const char         trimChar);

    static std::string trimTail(
        const std::string& source,
        const char         trimChar);

    static std::string trim(
        const std::string& source,
        const char         trimChar);

    static std::string& trimHeadLocal(
        std::string& source,
        const char   trimChar);

    static std::string& trimTailLocal(
        std::string& source,
        const char   trimChar);

    static std::string& trimLocal(
        std::string& source,
        const char   trimChar);

    /*!
    Add escape sequence prefix (\) to the char that is
    one of the regex reserved characters; otherwise return
    the same char string.
    */
    static std::string escapeRegexChar(const char regexChar);
};

} // namespace cadise