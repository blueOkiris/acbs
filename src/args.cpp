// Implement cli arg parsing

#include <string>
#include <variant>
#include <filesystem>
#include <err.hpp>
#include <args.hpp>

using namespace acbs;
using namespace acbs::args;

std::variant<CliArgs, err::AcbsErr> CliArgs::parse(const int argc, const char **argv) {
    if (argc < 2) {
        return err::AcbsErr::NoArgumentsProvided;
    }
    if (argc > 3) {
        return err::AcbsErr::TooManyArguments;
    }

    std::string ini = "acbs.ini";
    if (argc == 3) {
        ini = std::string(argv[2]) + "/acbs.ini";
    }
    if (!std::filesystem::exists(ini)) {
        return err::AcbsErr::NoIniFile;
    }

    if (std::string(argv[1]) == "build") {
        return CliArgs { .ini = ini, .cmd = Command::Build };
    } else {
        return err::AcbsErr::InvalidCommand;
    }
}

