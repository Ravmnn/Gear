#pragma once

#include <string>
#include <vector>
#include <map>



enum class TokenType
{
    Attribute, SemiColon, Colon, Arrow,
    Comma,

    Plus, Minus, Star, Slash, Equal,
    ParenLeft, ParenRight,
    
    Identifier,
    Value,
    Type,

    KwDeclare,
    KwStart, KwEnd, KwFunction, KwReturn,
};


struct TokenPosition
{
    unsigned int start;
    unsigned int end;
    unsigned int line;
};


struct Token
{
    inline static const std::vector<std::string> types = {
        "byte", "char", "bool", "uint8", "uint16",
        "uint32", "uint64", "int8", "int16", "int32",
        "int64"
    };


    inline static const std::map<std::string, TokenType> keywords = {
        {"decl", TokenType::KwDeclare},
        {"start", TokenType::KwStart},
        {"end", TokenType::KwEnd},
        {"function", TokenType::KwFunction},
        {"return", TokenType::KwReturn},
    };


    static bool isKeyword(const std::string& keyword) noexcept;
    static bool isType(const std::string& type) noexcept;
    static bool isBoolean(const std::string& boolean) noexcept;
    static bool stringToBoolean(const std::string& boolean) noexcept;


    bool isKeyword() const noexcept;
    bool isType() const noexcept;
    bool isBoolean() const noexcept;


    std::string lexeme;
    TokenPosition position;
    TokenType type;
};