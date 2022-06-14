#pragma once

#include <initializer_list>
#include <functional>
#include <regex>
#include <string>
#include <vector>

namespace cadise
{

class Tokenizer
{
private:
    using PostFunc = std::function<void(std::string& source)>;

public:
    static Tokenizer makeFromDelimiters(
        const std::initializer_list<char>& delimiters);

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
    explicit Tokenizer(const std::initializer_list<char>& delimiters);

    Tokenizer(
        const char openDelimiter, 
        const char closeDelimiter,
        const bool shouldIncludeEdges);

    void _splitThroughRegex(
        const std::string         source,
        std::vector<std::string>& out_substrings) const;

    std::regex _regex;
    PostFunc   _postFunc;
    bool       _isSeparator;
};

} // namespace cadise