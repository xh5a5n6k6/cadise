#pragma once

#include <functional>
#include <memory>
#include <string>
#include <map>

namespace cadise {

// TODO: replace map to unordered_map when STL supports heterogeneous lookup
//       in unordered_map
template<typename T>
using StringKeyMap = std::map<std::string, std::shared_ptr<T>, std::less<>>;

} // namespace cadise