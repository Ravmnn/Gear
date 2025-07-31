#pragma once

#include <string>
#include <iostream>



class GearException;


bool failed() noexcept;

void error(const std::string& message) noexcept;
void error(const GearException* exception);


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