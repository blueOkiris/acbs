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
        return err::AcbsErr { .type = err::AcbsErrType::NoArgumentsProvided, .extraInfo = "" };
    }
    if (argc > 3) {
        return err::AcbsErr { .type = err::AcbsErrType::TooManyArguments, .extraInfo = "" };
    }

    std::string ini = "acbs.ini";
    if (argc == 3) {
        ini = std::string(argv[2]) + "/acbs.ini";
    }
    if (!std::filesystem::exists(ini)) {
        return err::AcbsErr { .type = err::AcbsErrType::NoIniFile, .extraInfo = ini };
    }

    if (std::string(argv[1]) == "build") {
        return CliArgs { .ini = ini, .cmd = Command::Build };
    } else if (std::string(argv[1]) == "debug") {
        return CliArgs { .ini = ini, .cmd = Command::Debug };
    } else if (std::string(argv[1]) == "clean") {
        return CliArgs { .ini = ini, .cmd = Command::Clean };
    } else {
        return err::AcbsErr {
            .type = err::AcbsErrType::InvalidCommand,
            .extraInfo = std::string(argv[1])
        };
    }
}

