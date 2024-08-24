// Parse cli arguments into an easier-to-use struct

#include <iostream>
#include <string>
#include <variant>
#include <err.hpp>

#define VERSION     4

namespace acbs {
    namespace args {
        enum class Command {
            Build,
            Debug,
            Clean
        };

        struct CliArgs {
            public:
                static std::variant<CliArgs, err::AcbsErr> parse(
                    const int argc, const char **argv
                );

                const std::string ini;
                const Command cmd;
                const std::string workingDir;
        };

        static inline void printUsage(void) {
            std::cout
                << "ACBS v" << VERSION << std::endl
                << "by Dylan Turner" << std::endl
                << std::endl
                << "Usage:" << std::endl
                << "acbs COMMAND [FOLDER]" << std::endl
                << std::endl
                << "FOLDER          Folder containing an acbs.ini file. Default is './'"
                    << std::endl
                << std::endl
                << "Commands:" << std::endl
                << "- build         Build the project. Uses delta, so only builds what's changed"
                    << std::endl
                << "- debug         Build using your specified debug flags. Cleans first"
                    << std::endl
                << "- clean         Removes build files" << std::endl;
        }
    }
}

