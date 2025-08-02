#pragma once

#include <compiler/exceptions/locatable_exception.hpp>



class ParserException : public LocatableException
{
public:
    explicit ParserException(unsigned int id, const std::string& message, const TokenPosition& position) noexcept;
};