#include <compiler/exceptions/exceptions.hpp>



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




InternalException internal_e0000() noexcept
{
    return InternalException(0, "Invalid type name.");
}

InternalException internal_e0001() noexcept
{
    return InternalException(1, "Invalid binary operator.");
}

InternalException internal_e0002() noexcept
{
    return InternalException(2, "Unexpected null pointer.");
}

InternalException internal_e0003() noexcept
{
    return InternalException(3, "There are no busy registers.");
}

InternalException internal_e0004() noexcept
{
    return InternalException(4, "Register is already busy.");
}