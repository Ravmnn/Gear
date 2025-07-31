#include <compiler/exceptions/parser_exception.hpp>

#include <sstream>

#include <gear.hpp>
#include <compiler/exceptions/exception_formatting.hpp>



// TODO: creating an intermediate class for ParserException and ScannerException (like LocatableException) may be a good idea


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

    stream << formatExceptionInfo() << std::endl;
    stream << "  -> " << generateFinalExceptionMessage(message(), position());

    return stream.str();
}



std::string ParserException::formatExceptionInfo() const noexcept
{
    std::stringstream stream;

    const std::string filename = Gear::options().filePath.filename();

    stream << GearException::formatExceptionInfo() << " ";
    stream << '[' << filename << ':' << m_position.line << ", " << m_position.start << ';' << m_position.end << ']' << ' ';

    return stream.str();
}