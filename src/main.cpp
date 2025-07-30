#include <iostream>

#include <CLI11/CLI11.hpp>

#include <gear.hpp>



int main(int argc, char** argv)
{
    CLI::App app("Compiler for the language Gear.", "gear");
    

    GearOptions options;


    app.add_option("-f, --file, file", options.filePath, "The source file path.")
        ->required()
        ->check(CLI::ExistingFile);



    app.add_option("-e, --entrypoint", options.programEntryPoint, "The function that first runs when the program starts executing.");


    app.add_option("-b, --bitmode", options.programBitMode, "Whether to generate 16, 32, or 64 bit programs.")
        ->check(CLI::IsMember({ "16", "32", "64" }));


    try
    {
        app.parse(argc, argv);

        Gear::run(options);
    }

    catch (const CLI::ParseError &e)
    {
        return app.exit(e);
    }


    return 0;
}