#include "file-io/tokenizer.h"

#include "fundamental/assertion.h"
#include "fundamental/logger/logger.h"
#include "file-io/string_utils.h"

namespace cadise
{

namespace 
{
    const Logger logger("Tokenizer");
}

Tokenizer Tokenizer::makeFromDelimiters(
    const std::initializer_list<char>& delimiters)
{
    return Tokenizer(delimiters);
}

Tokenizer Tokenizer::makeFromOpenClosePattern(
    const char openDelimiter,
    const char closeDelimiter,
    const bool shouldIncludeEdges)
{
    return Tokenizer(openDelimiter, closeDelimiter, shouldIncludeEdges);
}

Tokenizer::Tokenizer(const std::initializer_list<char>& delimiters)
{
    CS_ASSERT_GT(delimiters.size(), 0);

    _isSeparator = true;
    _postFunc    = nullptr;

    std::string regexString = "";

    regexString += "[^";
    for (const auto& delimiter : delimiters)
    {
        regexString += delimiter;
    }
    regexString += "]+";

    _regex = std::regex(std::move(regexString));
}

Tokenizer::Tokenizer(
    const char openDelimiter,
    const char closeDelimiter,
    const bool shouldIncludeEdges)
{
    _isSeparator = false;
    _postFunc    = nullptr;
    if (!shouldIncludeEdges)
    {
        // Exclude open and close chars
        _postFunc = 
            [](std::string& source)
            {
                source = source.substr(1, source.length() - 2);
            };
    }

    std::string regexString = "";
    
    regexString += string_utils::escape_regex_char(openDelimiter);
    regexString += "(.*?)";
    regexString += string_utils::escape_regex_char(closeDelimiter);

    _regex = std::regex(std::move(regexString));
}

void Tokenizer::tokenize(
    const std::string&        source,
    std::vector<std::string>& out_tokens) const
{
    if (!_isSeparator)
    {
        logger.log(ELogLevel::Warn, "This tokenizer is not used for separation!");
        return;
    }

    _splitThroughRegex(source, out_tokens);
}

void Tokenizer::captureMatching(
    const std::string&        source,
    std::vector<std::string>& out_tokens) const
{
    if (_isSeparator)
    {
        logger.log(ELogLevel::Warn, "This tokenizer is not used for matching!");
        return;
    }

    _splitThroughRegex(source, out_tokens);
}

void Tokenizer::_splitThroughRegex(
    const std::string         source,
    std::vector<std::string>& out_substrings) const
{
    CS_ASSERT(out_substrings.empty());

    out_substrings.reserve(source.length());

    std::sregex_token_iterator iterator(source.begin(), source.end(), _regex);
    for (; iterator != std::sregex_token_iterator(); ++iterator)
    {
        std::string processedString = *iterator;
        if (_postFunc)
        {
            _postFunc(processedString);
        }

        out_substrings.push_back(std::move(processedString));
    }
}

} // namespace cadise