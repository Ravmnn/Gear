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
CompilerException gear_e3001(const TokenPosition& position) noexcept;
CompilerException gear_e3002(const TokenPosition& position) noexcept;
CompilerException gear_e3003(const TokenPosition& position) noexcept;
CompilerException gear_e3004(const TokenPosition& position) noexcept;



InternalException internal_e0000(const std::string& info = "") noexcept;
InternalException internal_e0000_nullptr() noexcept;
InternalException internal_e0000_argument() noexcept;
InternalException internal_e0001() noexcept;
InternalException internal_e0002() noexcept;
InternalException internal_e0003() noexcept;