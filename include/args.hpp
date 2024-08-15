// Parse cli arguments into an easier-to-use struct

#include <string>
#include <variant>
#include <err.hpp>

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
        };
    }
}

