#pragma once

#include <stack>

#include <compiler/language/token.hpp>



enum class ASMTypeSize;


struct Identifier
{
    std::string name;
    ASMTypeSize size;

    TokenPosition position;
};


class IdentifierManager
{
private:
    std::vector<Identifier> _identifiers;
    std::stack<unsigned int> _scopeMarkers;


public:
    IdentifierManager() = default;


    const std::vector<Identifier>& identifiers() const noexcept;


    void scopeBegin() noexcept;
    void scopeEnd() noexcept;

    unsigned int sizeOfCurrentScopeInBytes() const noexcept;


    bool isIdentifierDefined(const std::string& identifier) const noexcept;

    void defineIdentifier(const Identifier& identifier);
    
    const Identifier* getIdentifier(const std::string& identifier) const noexcept;


    void throwIfNotDefined(const std::string& identifier, const TokenPosition& position) const;
    void throwIfDefined(const std::string& identifier, const TokenPosition& position) const;
};