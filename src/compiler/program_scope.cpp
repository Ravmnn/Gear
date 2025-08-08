#include <compiler/program_scope.hpp>



IdentifierManager& ProgramScope::local() noexcept
{
    return _local;
}

IdentifierManager& ProgramScope::global() noexcept
{
    return _global;
}



bool ProgramScope::isIdentifierDefined(const std::string& identifier) const noexcept
{
    return _local.isIdentifierDefined(identifier) || _global.isIdentifierDefined(identifier);
}



const Identifier* ProgramScope::getIdentifier(const std::string& identifier) const noexcept
{
    if (_local.isIdentifierDefined(identifier))
        return _local.getIdentifier(identifier);

    if (_global.isIdentifierDefined(identifier))
        return _global.getIdentifier(identifier);

    return nullptr;
}



void ProgramScope::throwIfNotDefined(const std::string& identifier, const TokenPosition& position) const
{
    if (isIdentifierDefined(identifier))
        return;

    _local.throwIfNotDefined(identifier, position);
    _global.throwIfNotDefined(identifier, position);
}


void ProgramScope::throwIfDefined(const std::string& identifier, const TokenPosition& position) const
{
    if (!isIdentifierDefined(identifier))
        return;

    _local.throwIfDefined(identifier, position);
    _global.throwIfDefined(identifier, position);
}