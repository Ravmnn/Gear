#include <compiler/exceptions/parser_exception.hpp>

#include <sstream>

#include <compiler/exceptions/exception_formatting.hpp>



ParserException::ParserException(const unsigned int id, const std::string& message, const TokenPosition& position)
    : GearException((unsigned int)ExceptionSection::Parser, id, message), m_position(position)
{}


const TokenPosition& ParserException::position() const noexcept
{
    return m_position;
}


std::string ParserException::format() const noexcept
{
    std::stringstream stream;

    stream << formatExceptionInfo() << " -> " << generateFinalExceptionMessage(message(), position());

    return stream.str();
}