// Implement build functions

#include <iostream>
#include <optional>
#include <err.hpp>
#include <ini.hpp>
#include <build.hpp>

using namespace acbs;
using namespace acbs::build;

std::optional<err::AcbsErr> acbs::build::build(const ini::Project &proj, const bool isDebug) {
    std::cout << "Not implemented" << std::endl;
    return std::nullopt;
}

void acbs::build::clean(const ini::Project &proj) {
    std::cout << "Not implemented" << std::endl;
}

