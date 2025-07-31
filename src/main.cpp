#include <iostream>

#include <CLI11/CLI11.hpp>

#include <gear.hpp>



void addCommandLineOptions(CLI::App& app, GearOptions& options) noexcept;
void addCommandLineFlags(CLI::App& app, GearOptions& options) noexcept;



int main(int argc, char** argv)
{
    CLI::App app("Compiler for the language Gear.", "gear");
    GearOptions options;

    addCommandLineOptions(app, options);
    addCommandLineFlags(app, options);

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



void addCommandLineOptions(CLI::App& app, GearOptions& options) noexcept
{
    app.add_option("-f, --file, file", options.filePath, "The source file path.")
        ->required()
        ->check(CLI::ExistingFile);




    app.add_option("--entrypoint", options.programEntryPoint, "The function that first runs when the program starts executing.")
        ->capture_default_str();


    app.add_option("--bitmode", options.programBitMode, "Whether to generate 16, 32, or 64 bit programs.")
        ->check(CLI::IsMember({ "16", "32", "64" }))
        ->capture_default_str();


    app.add_option("-o, --output", options.programOutputName, "The name given to the output file.")
        ->capture_default_str();
}



void addCommandLineFlags(CLI::App& app, GearOptions& options) noexcept
{
    app.add_flag("--print-ast", options.printAst, "Whether to print the Abstract Syntactic Tree or not.");
    app.add_flag("--compile, !--no-compile", options.compile, "Whether to compile the source file or not.");
    app.add_flag("--assemble, !--no-assemble", options.assemble, "Whether to assemble the assembly generated or not.");
    app.add_flag("--link, !--no-link", options.link, "Whether to link the object file generated or not.");
}