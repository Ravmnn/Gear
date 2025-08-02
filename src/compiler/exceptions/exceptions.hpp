#pragma once

#include <compiler/exceptions/gear_exception.hpp>
#include <compiler/exceptions/scanner_exception.hpp>
#include <compiler/exceptions/parser_exception.hpp>
#include <compiler/exceptions/compiler_exception.hpp>
#include <compiler/exceptions/internal_exception.hpp>



ScannerException gear_e1000(const TokenPosition& position) noexcept;
ScannerException gear_e1001(const TokenPosition& position) noexcept;



ParserException gear_e2000(const TokenPosition& position) noexcept;
ParserException gear_e2001(const TokenPosition& position) noexcept;
ParserException gear_e2002(const TokenPosition& position) noexcept;
ParserException gear_e2003(const TokenPosition& position) noexcept;
ParserException gear_e2004(const TokenPosition& position) noexcept;
ParserException gear_e2005(const TokenPosition& position) noexcept;
ParserException gear_e2006(const TokenPosition& position) noexcept;
ParserException gear_e2007(const TokenPosition& position) noexcept;
ParserException gear_e2008(const TokenPosition& position) noexcept;
ParserException gear_e2009(const TokenPosition& position) noexcept;
ParserException gear_e2010(const TokenPosition& position) noexcept;
ParserException gear_e2011(const TokenPosition& position) noexcept;
ParserException gear_e2012(const TokenPosition& position) noexcept;



CompilerException gear_e3000(const TokenPosition& position) noexcept;



InternalException internal_e0000() noexcept;
InternalException internal_e0001() noexcept;
InternalException internal_e0002() noexcept;
InternalException internal_e0003() noexcept;
InternalException internal_e0004() noexcept;
InternalException internal_e0005() noexcept;