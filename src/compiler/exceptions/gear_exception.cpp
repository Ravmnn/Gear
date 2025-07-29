#include <compiler/exceptions/gear_exception.hpp>

#include <sstream>
#include <iomanip>



GearException::GearException(const unsigned int section, const unsigned int id, const std::string& message) noexcept
    : m_message(message), m_section(section), m_id(id)
{}


const std::string& GearException::message() const noexcept
{
    return m_message;
}


unsigned int GearException::id() const noexcept
{
    return m_id;
}


const char* GearException::what() const noexcept
{
    return m_message.c_str();
}



std::string GearException::formatExceptionInfo() const noexcept
{
    std::stringstream stream;

    stream << "GE:" << m_section << " ";
    stream << '(' << std::setw(3) << std::setfill('0') << std::right << m_id << ')';

    return stream.str();
}