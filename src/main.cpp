// Entry point for ACBS

#include <iostream>
#include <variant>
#include <args.hpp>
#include <ini.hpp>
#include <err.hpp>

using namespace acbs;

int main(int argc, const char **const argv) {
    const auto argsRes = args::CliArgs::parse(argc, argv);
    if (std::holds_alternative<err::AcbsErr>(argsRes)) {
        std::cerr << "Error: " << err::errToString(std::get<err::AcbsErr>(argsRes)) << std::endl;
        return 1;
    }
    const auto args = std::get<args::CliArgs>(argsRes);

    const auto projRes = ini::Project::parse(args.ini);
    if (std::holds_alternative<err::AcbsErr>(projRes)) {
        std::cerr << "Error: " << err::errToString(std::get<err::AcbsErr>(projRes)) << std::endl;
        return 1;
    }
    const auto proj = std::get<ini::Project>(projRes);
    std::cout
        << "Using project settings: " << std::endl
        << "[Compiler]" << std::endl
        << "Compiler = " << proj.compiler.compiler << std::endl
        << "Flags = " << proj.compiler.flags << std::endl
        << "Debug Flags = " << proj.compiler.debugFlags << std::endl
        << "Linker = " << proj.compiler.linker << std::endl
        << "Linker Flags = " << proj.compiler.linkerFlags << std::endl
        << "[Project]" << std::endl
        << "Name = " << proj.project.name << std::endl
        << "Is Bin? = " << proj.project.isBin << std::endl
        << "Is Cpp? = " << proj.project.isCpp << std::endl
        << "Include = {";
    for (const auto &inc : proj.project.include) {
        std::cout << " " << inc;
    }
    std::cout
        << " }" << std::endl
        << "Source = {";
    for (const auto &src : proj.project.source) {
        std::cout << " " << src;
    }
    std::cout
        << " }" << std::endl
        << "Build = " << proj.project.build << std::endl;

    switch (args.cmd) {
        case args::Command::Build:
            std::cout << "TODO: Build" << std::endl;
            break;
        case args::Command::Debug:
            std::cout << "TODO: Debug" << std::endl;
            break;
        case args::Command::Clean:
            std::cout << "TODO: Clean" << std::endl;
            break;
    }
    return 0;
}

