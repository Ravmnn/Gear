#pragma once

#include <compiler/language/token.hpp>



enum class ASMTypeSize;


struct Identifier
{
    const std::string name;
    const ASMTypeSize size;

    const TokenPosition position;
};


class IdentifierManager
{
private:
    std::vector<Identifier> _identifiers;


public:
    IdentifierManager() = default;


    const std::vector<Identifier>& identifiers() const noexcept;


    bool isIdentifierDefined(const std::string& identifier) const noexcept;

    void defineIdentifier(const Identifier& identifier);
    
    const Identifier* getIdentifier(const std::string& identifier) const noexcept;


    void throwIfNotDefined(const std::string& identifier, const TokenPosition& position) const;
    void throwIfDefined(const std::string& identifier, const TokenPosition& position) const;
};