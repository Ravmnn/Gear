#pragma once

#include <compiler/exceptions/gear_exception.hpp>
#include <compiler/exceptions/scanner_exception.hpp>
#include <compiler/exceptions/parser_exception.hpp>



ScannerException gear_e1000(const TokenPosition& position) noexcept;
ScannerException gear_e1001(const TokenPosition& position) noexcept;



ParserException gear_e2000(const TokenPosition& position) noexcept;
ParserException gear_e2001(const TokenPosition& position) noexcept;
ParserException gear_e2002(const TokenPosition& position) noexcept;
ParserException gear_e2003(const TokenPosition& position) noexcept;
ParserException gear_e2004(const TokenPosition& position) noexcept;
ParserException gear_e2005(const TokenPosition& position) noexcept;