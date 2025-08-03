#include <compiler/identifiers.hpp>

#include <compiler/exceptions/exceptions.hpp>



const std::vector<Identifier>& IdentifierManager::identifiers() const noexcept
{
    return _identifiers;
}



void IdentifierManager::scopeBegin() noexcept
{
    _scopeMarkers.push(_identifiers.size());
}


void IdentifierManager::scopeEnd() noexcept
{
    const unsigned int marker = _scopeMarkers.top();
    _scopeMarkers.pop();

    if (_identifiers.size() > marker)
        _identifiers.erase(_identifiers.cbegin() + marker);
}



unsigned int IdentifierManager::sizeOfCurrentScopeInBytes() const noexcept
{
    unsigned int sizeInBytes = 0;

    for (const Identifier& identifier : _identifiers)
        sizeInBytes += (unsigned int)identifier.size;

    return sizeInBytes;
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