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
    std::string workingDir = ".";
    if (argc == 3) {
        ini = std::string(argv[2]) + "/acbs.ini";
        workingDir = std::string(argv[2]);
    }
    if (!std::filesystem::exists(ini)) {
        return err::AcbsErr { .type = err::AcbsErrType::NoIniFile, .extraInfo = ini };
    }

    if (std::string(argv[1]) == "build") {
        return CliArgs { .ini = ini, .cmd = Command::Build, .workingDir = workingDir };
    } else if (std::string(argv[1]) == "debug") {
        return CliArgs { .ini = ini, .cmd = Command::Debug, .workingDir = workingDir };
    } else if (std::string(argv[1]) == "clean") {
        return CliArgs { .ini = ini, .cmd = Command::Clean, .workingDir = workingDir };
    } else {
        return err::AcbsErr {
            .type = err::AcbsErrType::InvalidCommand,
            .extraInfo = std::string(argv[1])
        };
    }
}

