#pragma once

#include <string>
#include <vector>



class ScannerException;
class ParserException;


class Gear
{
private:
    static std::string s_source;
    static bool s_failed;


public:
    Gear() = delete;


    static const std::string& source() noexcept;
    static const std::vector<std::string> sourceAsLines() noexcept;


    static void compile(const std::string& source);


    static bool failed() noexcept;

    static void error(const std::string& message) noexcept;
    static void error(const ScannerException& exception) noexcept;
    static void error(const ParserException& exception) noexcept;
};