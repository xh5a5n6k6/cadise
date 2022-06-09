#pragma once

#include <initializer_list>
#include <regex>
#include <string>
#include <vector>

namespace cadise
{

class Tokenizer
{
public:
    static Tokenizer makeFromDelimiters(
        const std::initializer_list<char>& delimiters);

    static Tokenizer makeFromOpenClosePattern(
        const char openDelimiter,
        const char closeDelimiter);

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
        const char closeDelimiter);

    void _splitThroughRegex(
        const std::string         source,
        std::vector<std::string>& out_substrings) const;

    std::regex _regex;
    bool       _isSeparator;
};

} // namespace cadise