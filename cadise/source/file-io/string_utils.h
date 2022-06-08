#pragma once

#include <string>

/*
    Some string utilities

    - string trimming
    Reference: https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
*/
namespace cadise::string_utils
{

std::string trim_head(
    const std::string& source,
    const char         trimChar);

std::string trim_tail(
    const std::string& source,
    const char         trimChar);

std::string trim(
    const std::string& source,
    const char         trimChar);

std::string& trim_head_local(
    std::string& source,
    const char   trimChar);

std::string& trim_tail_local(
    std::string& source,
    const char   trimChar);

std::string& trim_local(
    std::string& source,
    const char   trimChar);

/*!
Add escape sequence prefix (\) to the char that is
one of the regex reserved characters; otherwise return
the same char string.
*/
std::string escape_regex_char(const char regexChar);

} // namespace cadise::string_utils