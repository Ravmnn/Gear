#pragma once

#include <compiler/identifiers.hpp>



class ProgramScope
{
private:
    IdentifierManager _local;
    IdentifierManager _global;


public:
    ProgramScope() = default;


    IdentifierManager& local() noexcept;
    IdentifierManager& global() noexcept;


    bool isIdentifierDefined(const std::string& identifier) const noexcept;


    const Identifier* getIdentifier(const std::string& identifier) const noexcept;


    void throwIfNotDefined(const std::string& identifier, const TokenPosition& position) const;
    void throwIfDefined(const std::string& identifier, const TokenPosition& position) const;
};