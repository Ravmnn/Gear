#include <compiler/exceptions/scanner_exception.hpp>



ScannerException::ScannerException(const unsigned int id, const std::string& message, const TokenPosition& position) noexcept
    : LocatableException((unsigned int)ExceptionSection::Scanner, id, message, position)
{}