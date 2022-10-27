#include "file-io/tokenizer.h"

#include "fundamental/assertion.h"
#include "fundamental/logger/logger.h"

namespace cadise
{

namespace 
{
    const Logger logger("Tokenizer");
}

Tokenizer Tokenizer::makeFromDelimiters(const std::initializer_list<char>& delimiters)
{
    return Tokenizer(EMode::Separator, delimiters, {});
}

Tokenizer Tokenizer::makeFromOpenClosePattern(
    const char openDelimiter,
    const char closeDelimiter,
    const bool shouldIncludeEdges)
{
    CS_ASSERT_NE(openDelimiter, closeDelimiter);

    const EMode mode = shouldIncludeEdges
        ? EMode::ClosureWithEdges
        : EMode::ClosureWithoutEdges;

    return Tokenizer(mode, {},  { openDelimiter, closeDelimiter });
}

Tokenizer::Tokenizer(
    const EMode                        mode,
    const std::initializer_list<char>& delimiters,
    const std::pair<char, char>&       openCloseDelimiterPair) :

    _mode(mode)
{
    switch (mode)
    {
        case EMode::Separator:
            if (std::empty(delimiters))
            {
                logger.log(ELogLevel::Warn, "Detect zero-length delimiters!");
            }

            _delimiters = std::unordered_set<char>(delimiters);
            break;

        case EMode::ClosureWithEdges:
        case EMode::ClosureWithoutEdges:
            _openCloseDelimiterPair = openCloseDelimiterPair;
            break;
    }
}

void Tokenizer::tokenize(
    const std::string&        source,
    std::vector<std::string>& out_tokens) const
{
    if (_mode != EMode::Separator)
    {
        logger.log(ELogLevel::Warn, "This tokenizer is not used for separation!");
        return;
    }

    for (std::size_t i = 0; i < source.length(); ++i)
    {
        if (_delimiters.contains(source[i]))
        {
            continue;
        }

        std::size_t endIndex = i + 1;
        while (endIndex < source.length())
        {
            if (_delimiters.contains(source[endIndex]))
            {
                break;
            }

            ++endIndex;
        }
        
        std::string token = source.substr(i, endIndex - i);
        out_tokens.push_back(std::move(token));

        i = endIndex;
    }
}

void Tokenizer::captureMatching(
    const std::string&        source,
    std::vector<std::string>& out_matchings) const
{
    if (_mode != EMode::ClosureWithEdges && _mode != EMode::ClosureWithoutEdges)
    {
        logger.log(ELogLevel::Warn, "This tokenizer is not used for matching!");
        return;
    }

    const auto [openChar, closeChar] = _openCloseDelimiterPair;
    for (std::size_t i = 0; i < source.length(); ++i)
    {
        if (source[i] == openChar)
        {
            std::size_t openIndex  = i;
            std::size_t closeIndex = openIndex + 1;
            while (closeIndex < source.length())
            {
                if (source[closeIndex] == openChar)
                {
                    openIndex = closeIndex;
                }
                else if (source[closeIndex] == closeChar)
                {
                    std::size_t leftIndex  = openIndex;
                    std::size_t rightIndex = closeIndex;
                    if (_mode == EMode::ClosureWithoutEdges)
                    {
                        ++leftIndex;
                        --rightIndex;
                    }

                    std::string matching = source.substr(
                        leftIndex,
                        rightIndex - leftIndex + 1);
                    out_matchings.push_back(std::move(matching));

                    // Move step to next index after finding valid matching
                    i = closeIndex;
                    break;
                }

                ++closeIndex;
            }
        }
    }
}

} // namespace cadise