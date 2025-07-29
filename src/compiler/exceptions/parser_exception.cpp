#include <compiler/exceptions/parser_exception.hpp>



ParserException::ParserException(const unsigned int id, const std::string& message, const TokenPosition& position)
    : GearException((unsigned int)ExceptionSection::Parser, id, message), m_position(position)
{}


const TokenPosition& ParserException::position() const noexcept
{
    return m_position;
}