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
    return ParserException(1, "Expect variable name after \"decl\" keyword.", position);
}

ParserException gear_e2002(const TokenPosition& position) noexcept
{
    return ParserException(2, "Expect type specifier (\":\") after variable name.", position);
}

ParserException gear_e2003(const TokenPosition& position) noexcept
{
    return ParserException(3, "Expect type name after type specifier.", position);
}

ParserException gear_e2004(const TokenPosition& position) noexcept
{
    return ParserException(4, "Expect assignment operator (\"=\") after variable type.", position);
}

ParserException gear_e2005(const TokenPosition& position) noexcept
{
    return ParserException(4, "Expect variable value after assignment operator (\"=\").", position);
}