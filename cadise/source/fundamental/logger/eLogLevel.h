#pragma once

namespace cadise 
{

/*! @brief An label to distinguish how important a log message is.
*/
enum class ELogLevel 
{
    Undefined = 0,

    Trace,
    Debug,
    Info,
    Warn,
    Error,
};

} // namespace cadise