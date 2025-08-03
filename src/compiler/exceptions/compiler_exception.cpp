#include <compiler/exceptions/compiler_exception.hpp>



CompilerException::CompilerException(const unsigned int id, const std::string& message, const TokenPosition& position) noexcept
    : LocatableException((unsigned int)ExceptionSection::Compiler, id, message, position)
{}