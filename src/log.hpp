#pragma once

#include <string>
#include <iostream>



class TorqueException;


bool failed() noexcept;

void error(const std::string& message) noexcept;
void error(const TorqueException* exception);


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