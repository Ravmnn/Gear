#include <compiler/exceptions/gear_exception.hpp>



GearException::GearException(const std::string& message) noexcept
    : _message(message)
{}


const char* GearException::what() const noexcept
{
    return _message.c_str();
}