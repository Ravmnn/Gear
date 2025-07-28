#include <compiler/exceptions/scanner_exceptions.hpp>



ScannerException::ScannerException(const std::string& message, const TokenPosition& position)
    : GearException(message), _position(position)
{}


const char* ScannerException::what() const noexcept
{
    
}



InvalidTokenException::InvalidTokenException(const TokenPosition& position)
    : ScannerException("Invalid token.", position)
{}