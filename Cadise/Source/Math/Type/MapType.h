#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace cadise
{

template<typename T>
using TStringKeyMap = std::unordered_map<std::string, std::shared_ptr<T>>;

} // namespace cadise