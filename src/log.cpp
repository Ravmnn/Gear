#include <log.hpp>

#include <iostream>

#include <compiler/exceptions/scanner_exception.hpp>
#include <compiler/exceptions/parser_exception.hpp>



static bool s_failed;

bool failed() noexcept
{
    return s_failed;
}



void error(const std::string& message) noexcept
{
    std::cerr << "Error: " << message << std::endl << std::endl;

    s_failed = true;
}


void error(const ScannerException& exception) noexcept
{
    error(exception.format());
}

void error(const ParserException& exception) noexcept
{
    error(exception.format());
}