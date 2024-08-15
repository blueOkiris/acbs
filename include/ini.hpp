// Parse the project inis

#pragma once

#include <string>
#include <vector>
#include <variant>
#include <err.hpp>

namespace acbs {
    namespace ini {
        struct CompilerSection {
            const std::string compiler;
            const std::string flags;
            const std::string debugFlags;
            const std::string linker;
            const std::string linkerFlags;
        };

        struct ProjectSection {
            const std::string name;
            const bool isBin;
            const bool isCpp;
            const std::vector<std::string> include;
            const std::vector<std::string> source;
            const std::string build;
        };

        struct Project {
            public:
                static std::variant<Project, err::AcbsErr> parse(const std::string &fileName);

                const CompilerSection compiler;
                const ProjectSection project;
        };
    }
}

