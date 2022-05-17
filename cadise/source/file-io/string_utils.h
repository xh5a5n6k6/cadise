#pragma once

#include <string>

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

} // namespace cadise::string_utils