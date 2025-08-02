#include <compiler/exceptions/locatable_exception.hpp>

#include <sstream>

#include <gear.hpp>
#include <compiler/exceptions/exception_formatting.hpp>



LocatableException::LocatableException(unsigned int section, unsigned int id, const std::string& message, const TokenPosition& position) noexcept
    : GearException(section, id, message), m_position(position)
{}



const TokenPosition& LocatableException::position() const noexcept
{
    return m_position;
}



std::string LocatableException::format() const noexcept
{
    std::stringstream stream;

    stream << formatExceptionInfo() << std::endl;
    stream << "  -> " << generateFinalExceptionMessage(message(), position());

    return stream.str();
}



std::string LocatableException::formatExceptionInfo() const noexcept
{
    std::stringstream stream;

    const std::string filename = Gear::options().filePath.filename();

    stream << GearException::formatExceptionInfo() << " ";
    stream << '[' << filename << ':' << m_position.line << ", " << m_position.start << ';' << m_position.end << ']' << ' ';

    return stream.str();
}