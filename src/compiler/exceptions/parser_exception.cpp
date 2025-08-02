#include <compiler/exceptions/parser_exception.hpp>



ParserException::ParserException(const unsigned int id, const std::string& message, const TokenPosition& position) noexcept
    : LocatableException((unsigned int)ExceptionSection::Parser, id, message, position)
{}