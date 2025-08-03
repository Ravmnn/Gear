#include <compiler/exceptions/exceptions.hpp>

#include <sstream>



ScannerException gear_e1000(const TokenPosition& position) noexcept
{
    return ScannerException(0, "Invalid token.", position);
}

ScannerException gear_e1001(const TokenPosition& position) noexcept
{
    return ScannerException(1, "Unclosed multiline comment.", position);
}





ParserException gear_e2000(const TokenPosition& position) noexcept
{
    return ParserException(0, "Expect \";\" after statement.", position);
}

ParserException gear_e2001(const TokenPosition& position) noexcept
{
    return ParserException(1, "Expect identifier.", position);
}

ParserException gear_e2002(const TokenPosition& position) noexcept
{
    return ParserException(2, "Expect type specifier.", position);
}

ParserException gear_e2003(const TokenPosition& position) noexcept
{
    return ParserException(3, "Expect type name.", position);
}

ParserException gear_e2004(const TokenPosition& position) noexcept
{
    return ParserException(4, "Expect assignment operator.", position);
}

ParserException gear_e2005(const TokenPosition& position) noexcept
{
    return ParserException(5, "Unclosed grouping expression.", position);
}

ParserException gear_e2006(const TokenPosition& position) noexcept
{
    return ParserException(6, "Expect expression.", position);
}

ParserException gear_e2007(const TokenPosition& position) noexcept
{
    return ParserException(7, "Expect \"(\" after function name.", position);
}

ParserException gear_e2008(const TokenPosition& position) noexcept
{
    return ParserException(8, "Expect \")\" before return type specifier.", position);
}

ParserException gear_e2009(const TokenPosition& position) noexcept
{
    return ParserException(9, "Expect function return type specifier after parameter list.", position);
}

ParserException gear_e2010(const TokenPosition& position) noexcept
{
    return ParserException(10, "Expect block statement.", position);
}

ParserException gear_e2011(const TokenPosition& position) noexcept
{
    return ParserException(11, "Unclosed block statement.", position);
}

ParserException gear_e2012(const TokenPosition& position) noexcept
{
    return ParserException(12, "Wrong keyword placement.", position);
}





CompilerException gear_e3000(const TokenPosition& position) noexcept
{
    return CompilerException(0, "File scope should only contain function declarations.", position);
}

CompilerException gear_e3001(const TokenPosition& position) noexcept
{
    return CompilerException(1, "Undefined identifer.", position);
}

CompilerException gear_e3002(const TokenPosition& position) noexcept
{
    return CompilerException(2, "Identifier is already defined.", position);
}

CompilerException gear_e3003(const TokenPosition& position) noexcept
{
    return CompilerException(3, "Functions should only be declared at file scope.", position);
}

CompilerException gear_e3004(const TokenPosition& position) noexcept
{
    return CompilerException(4, "Operation depth is too complex and big, overpassed max register count.", position);
}





InternalException internal_e0000(const std::string& info) noexcept
{
    std::stringstream stream;

    stream << "Bad API usage.";
    stream << " " << '(' << info << ')';

    return InternalException(0, stream.str());
}

InternalException internal_e0000_nullptr() noexcept
{
    return internal_e0000("Unexpected nullptr.");
}

InternalException internal_e0000_argument() noexcept
{
    return internal_e0000("Invalid argument.");
}


InternalException internal_e0001() noexcept
{
    return InternalException(1, "There are no busy registers.");
}

InternalException internal_e0002() noexcept
{
    return InternalException(2, "Register is already busy.");
}

InternalException internal_e0003() noexcept
{
    return InternalException(3, "There are no free registers.");
}