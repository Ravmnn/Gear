#pragma once

#include <string>
#include <vector>



struct Token;
enum class TokenType;


class Scanner
{
private:
    std::vector<Token> _tokens;

    std::string _source;
    TokenPosition _position; // TODO: update this
    unsigned int _start;
    unsigned int _end;


public:
    explicit Scanner(const std::string& source) noexcept;


    std::vector<Token> scan();


private:
    void scanToken();

    void addToken(TokenType type) noexcept;

    std::string getSourceCurrentSubstring() const noexcept;


    void reset() noexcept;


    void identifier() noexcept;
    void value() noexcept;

    void comment() noexcept;
    void multilineComment() noexcept;


    char advance() noexcept;
    char peek() const noexcept;
    char peekNext() const noexcept;
    bool atEnd() const noexcept;
};