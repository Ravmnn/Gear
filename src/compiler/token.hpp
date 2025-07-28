#pragma once

#include <string>
#include <map>



enum class TokenType
{
    Attribute, SemiColon, Colon, Arrow,

    Plus, Minus, Star, Slash, Equal,
    ParenLeft, ParenRight,
    
    Identifier,
    Value,

    KwTypeByte, KwTypeChar, KwTypeBool,
    KwTypeUint8, KwTypeUint16, KwTypeUint32, KwTypeUint64,
    KwTypeInt8, KwTypeInt16, KwTypeInt32, KwTypeInt64,

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
    inline static const std::map<std::string, TokenType> keywords = {
        {"byte", TokenType::KwTypeByte},
        {"char", TokenType::KwTypeChar},
        {"bool", TokenType::KwTypeBool},
        {"uint8", TokenType::KwTypeUint8},
        {"uint16", TokenType::KwTypeUint16},
        {"uint32", TokenType::KwTypeUint32},
        {"uint64", TokenType::KwTypeUint64},
        {"int8", TokenType::KwTypeInt8},
        {"int16", TokenType::KwTypeInt16},
        {"int32", TokenType::KwTypeInt32},
        {"int64", TokenType::KwTypeInt64},
        {"decl", TokenType::KwDeclare},
        {"start", TokenType::KwStart},
        {"end", TokenType::KwEnd},
        {"function", TokenType::KwFunction},
        {"return", TokenType::KwReturn},
    };


    static bool isKeyword(const std::string& keyword) noexcept;
    static bool isBoolean(const std::string& boolean) noexcept;
    static bool stringToBoolean(const std::string& boolean) noexcept;


    bool isKeyword() const noexcept;
    bool isBoolean() const noexcept;


    std::string lexeme;
    TokenPosition position;
    TokenType type;
};