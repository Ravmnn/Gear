#include <compiler/exceptions/scanner_exception.hpp>

#include <compiler/exceptions/exception_formatting.hpp>



ScannerException::ScannerException(const unsigned int id, const std::string& message, const TokenPosition& position)
    : GearException((unsigned int)ExceptionSection::Scanner, id, message), m_position(position)
{}


const TokenPosition& ScannerException::position() const noexcept
{
    return m_position;
}