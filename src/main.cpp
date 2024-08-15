// Entry point for ACBS

#include <iostream>
#include <variant>
#include <args.hpp>
#include <err.hpp>

using namespace acbs;

int main(int argc, const char **const argv) {
    const auto argsRes = args::CliArgs::parse(argc, argv);
    if (std::holds_alternative<err::AcbsErr>(argsRes)) {
        std::cerr << "Error: " << err::errToString(std::get<err::AcbsErr>(argsRes)) << std::endl;
        return 1;
    }
    const auto args = std::get<args::CliArgs>(argsRes);
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

