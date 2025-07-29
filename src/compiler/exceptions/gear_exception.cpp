#include <compiler/exceptions/gear_exception.hpp>



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