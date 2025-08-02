#pragma once

#include <compiler/exceptions/locatable_exception.hpp>



class CompilerException : public LocatableException
{
public:
    explicit CompilerException(unsigned int id, const std::string& message, const TokenPosition& position) noexcept;
};