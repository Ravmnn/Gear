#include <compiler/exceptions/internal_exception.hpp>

#include <sstream>



InternalException::InternalException(const unsigned int code, const std::string& message) noexcept
    : GearException((unsigned int)ExceptionSection::Internal, code, message)
{}


std::string InternalException::format() const noexcept
{
    std::stringstream stream;

    stream << formatExceptionInfo() << std::endl;
    stream << "  -> " << message();

    return stream.str();
}



std::string InternalException::formatExceptionInfo() const noexcept
{
    std::stringstream stream;

    stream << GearException::formatExceptionInfo() << " ";
    return stream.str();
}