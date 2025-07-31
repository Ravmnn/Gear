#include <log.hpp>

#include <iostream>

#include <compiler/exceptions/scanner_exception.hpp>
#include <compiler/exceptions/parser_exception.hpp>
#include <compiler/exceptions/exceptions.hpp>



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


void error(const GearException* const exception)
{
    if (!exception)
        throw internal_e0002();

    error(exception->format());
}