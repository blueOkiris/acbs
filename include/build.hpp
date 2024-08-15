// API for running build commands

#pragma once

#include <optional>
#include <err.hpp>
#include <ini.hpp>

namespace acbs {
    namespace build {
        std::optional<err::AcbsErr> build(const ini::Project &proj, const bool isDebug);
        void clean(const ini::Project &proj);
    }
}

