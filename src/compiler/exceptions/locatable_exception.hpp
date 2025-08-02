#pragma once

#include <compiler/exceptions/gear_exception.hpp>
#include <compiler/language/token.hpp>



class LocatableException : public GearException
{
protected:
    const TokenPosition m_position;


public:
    LocatableException(unsigned int section, unsigned int id, const std::string& message, const TokenPosition& position) noexcept;


    const TokenPosition& position() const noexcept;


    std::string format() const noexcept override;


protected:
    std::string formatExceptionInfo() const noexcept override;
};