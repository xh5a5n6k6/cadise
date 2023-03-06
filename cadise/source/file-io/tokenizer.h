#pragma once

#include <initializer_list>
#include <string>
#include <unordered_set>

namespace cadise
{

class Tokenizer
{
private:
    enum class EMode
    {
        Undefined = 0,

        Separator,
        ClosureWithEdges,
        ClosureWithoutEdges,
    };

public:
    static Tokenizer makeFromDelimiters(const std::initializer_list<char>& delimiters);

    static Tokenizer makeFromOpenClosePattern(
        const char openDelimiter,
        const char closeDelimiter,
        const bool shouldIncludeEdges);

public:
    void tokenize(
        const std::string&        source,
        std::vector<std::string>& out_tokens) const;

    void captureMatching(
        const std::string&        source,
        std::vector<std::string>& out_matchings) const;

private:
    Tokenizer(
        const EMode                        mode,
        const std::initializer_list<char>& delimiters,
        const std::pair<char, char>&       openCloseDelimiterPair);

    EMode                    _mode;
    std::unordered_set<char> _delimiters;
    std::pair<char, char>    _openCloseDelimiterPair;
};

} // namespace cadise