#include <compiler/exceptions/scanner_exception.hpp>

#include <sstream>

#include <gear.hpp>
#include <compiler/exceptions/exception_formatting.hpp>



ScannerException::ScannerException(const unsigned int id, const std::string& message, const TokenPosition& position)
    : GearException((unsigned int)ExceptionSection::Scanner, id, message), m_position(position)
{}


const TokenPosition& ScannerException::position() const noexcept
{
    return m_position;
}


std::string ScannerException::format() const noexcept
{
    std::stringstream stream;

    stream << formatExceptionInfo() << std::endl;
    stream << "  -> " << generateFinalExceptionMessage(message(), position());

    return stream.str();
}



std::string ScannerException::formatExceptionInfo() const noexcept
{
    std::stringstream stream;

    const std::string filename = Gear::options().filePath.filename();

    stream << GearException::formatExceptionInfo() << " ";
    stream << '[' << filename << ':' << m_position.line << ", " << m_position.start << ';' << m_position.end << ']' << ' ';

    return stream.str();
}