#pragma once

#include <string>
#include <iostream>



class CommandLineException;
class ScannerException;
class ParserException;


bool failed() noexcept;

void error(const std::string& message) noexcept;
void error(const ScannerException& exception) noexcept;
void error(const ParserException& exception) noexcept;


template <typename... T>
void print(const T&... values)
{
    (std::cout << ... << values);
}


template <typename... T>
void println(const T&... values)
{
    (std::cout << ... << values) << std::endl;
}