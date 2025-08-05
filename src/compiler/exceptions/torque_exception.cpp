#include <compiler/exceptions/torque_exception.hpp>

#include <sstream>
#include <iomanip>

#include <torque.hpp>



TorqueException::TorqueException(const unsigned int section, const unsigned int id, const std::string& message) noexcept
    : m_message(message), m_section(section), m_id(id)
{}


const std::string& TorqueException::message() const noexcept
{
    return m_message;
}


unsigned int TorqueException::id() const noexcept
{
    return m_id;
}


const char* TorqueException::what() const noexcept
{
    return format().c_str();
}



std::string TorqueException::formatExceptionInfo() const noexcept
{
    std::stringstream stream;

    stream << "GE:" << m_section << " ";
    stream << '(' << std::setw(3) << std::setfill('0') << std::right << m_id << ')';

    return stream.str();
}