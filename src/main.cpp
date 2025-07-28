#include <iostream>

#include <file.hpp>
#include <gear.hpp>



int main(int argc, char** argv)
{
    const std::string source = readFile("examples/main.gr");

    Gear::compile(source);

    return 0;
}