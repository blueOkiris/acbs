// Entry point for ACBS

#include <filesystem>
#include <iostream>
#include <variant>
#include <args.hpp>
#include <ini.hpp>
#include <err.hpp>
#include <build.hpp>
#include <unistd.h>

using namespace acbs;

int main(int argc, const char **const argv) {
    const auto argsRes = args::CliArgs::parse(argc, argv);
    if (std::holds_alternative<err::AcbsErr>(argsRes)) {
        std::cerr << "Error: " << err::errToString(std::get<err::AcbsErr>(argsRes)) << std::endl;
        args::printUsage();
        return 1;
    }
    const auto args = std::get<args::CliArgs>(argsRes);

    const auto projRes = ini::Project::parse(args.workingDir, args.ini);
    if (std::holds_alternative<err::AcbsErr>(projRes)) {
        std::cerr << "Error: " << err::errToString(std::get<err::AcbsErr>(projRes)) << std::endl;
        return 1;
    }
    const auto proj = std::get<ini::Project>(projRes);

    const auto startPath = std::filesystem::current_path();
    if (proj.project.workingDir != ".") {
        if (chdir(proj.project.workingDir.c_str()) < 0) {
            std::cerr << "NOTE: Could not set working directory!" << std::endl;
        }
        std::cout
            << "Changed into working directory '" << std::filesystem::current_path() << "'"
            << std::endl << std::endl;
    }
    switch (args.cmd) {
        case args::Command::Build: {
            const auto buildErr = build::build(proj, false);
            if (buildErr.has_value()) {
                std::cerr << "Error: " << err::errToString(buildErr.value()) << std::endl;
                if (proj.project.workingDir != ".") {
                    if (chdir(startPath.c_str()) < 0) {
                        std::cerr << "NOTE: Could not return to original working directory!" << std::endl;
                    }
                }
                return 1;
            }
            break;
        }
        case args::Command::Debug: {
            build::clean(proj);
            const auto buildErr = build::build(proj, true);
            if (buildErr.has_value()) {
                std::cerr << "Error: " << err::errToString(buildErr.value()) << std::endl;
                if (proj.project.workingDir != ".") {
                    if (chdir(startPath.c_str()) < 0) {
                        std::cerr << "NOTE: Could not return to original working directory!" << std::endl;
                    }
                }
                return 1;
            }
            break;
        }
        case args::Command::Clean:
            build::clean(proj);
            break;
    }
    if (proj.project.workingDir != ".") {
        if (chdir(startPath.c_str()) < 0) {
            std::cerr << "NOTE: Could not return to original working directory!" << std::endl;
        }
    }
    return 0;
}

