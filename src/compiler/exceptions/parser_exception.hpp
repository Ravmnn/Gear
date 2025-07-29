#pragma once

#include <compiler/token.hpp>
#include <compiler/exceptions/gear_exception.hpp>



class ParserException : public GearException
{
protected:
    const TokenPosition m_position;


public:
    explicit ParserException(unsigned int id, const std::string& message, const TokenPosition& position);


    const TokenPosition& position() const noexcept;


    std::string format() const noexcept;
};