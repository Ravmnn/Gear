#include <compiler/identifiers.hpp>

#include <compiler/exceptions/exceptions.hpp>


// TODO: don't forget to remove the identifiers removed from scope.


const std::vector<Identifier>& IdentifierManager::identifiers() const noexcept
{
    return _identifiers;
}



bool IdentifierManager::isIdentifierDefined(const std::string& identifier) const noexcept
{
    for (const Identifier& definedIdentifier : _identifiers)
        if (definedIdentifier.name == identifier)
            return true;

    return false;
}



void IdentifierManager::defineIdentifier(const Identifier& identifier)
{
    throwIfDefined(identifier.name, identifier.position);

    _identifiers.push_back(identifier);
}


const Identifier* IdentifierManager::getIdentifier(const std::string& identifier) const noexcept
{
    for (const Identifier& declaredIdentifier : _identifiers)
        if (declaredIdentifier.name == identifier)
            return &declaredIdentifier;

    return nullptr;
}





void IdentifierManager::throwIfNotDefined(const std::string& identifier, const TokenPosition& position) const
{
    if (!isIdentifierDefined(identifier))
        throw gear_e3001(position);
}


void IdentifierManager::throwIfDefined(const std::string& identifier, const TokenPosition& position) const
{
    if (isIdentifierDefined(identifier))
        throw gear_e3002(position);
}