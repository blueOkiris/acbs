// Implement build functions

#include <iostream>
#include <chrono>
#include <optional>
#include <vector>
#include <filesystem>
#include <sstream>
#include <err.hpp>
#include <ini.hpp>
#include <build.hpp>

using namespace acbs;
using namespace acbs::build;

static std::vector<std::pair<std::filesystem::path, std::filesystem::path>> getDeltaFiles(
    const ini::Project &proj
);
static std::string buildFileCmd(
    const ini::Project &proj, const bool isDebug,
    const std::filesystem::path &src, const std::filesystem::path &obj
);
static std::string linkObjsCmd(const ini::Project &proj);

std::optional<err::AcbsErr> acbs::build::build(const ini::Project &proj, const bool isDebug) {
    // Get and show the users what files will be built
    const auto srcObjZip = getDeltaFiles(proj);
    std::cout << "Files queued for building:" << std::endl;
    for (const auto &fnamePair : srcObjZip) {
        std::cout << "- " << fnamePair.first << " into " << fnamePair.second << std::endl;
    }
    std::cout << std::endl;

    for (const auto &srcObj : srcObjZip) {
        std::cout << "Building " << srcObj.second << ":" << std::endl;
        const auto cmd = buildFileCmd(proj, isDebug, srcObj.first, srcObj.second);
        std::cout << "- Command: '" << cmd << "'" << std::endl;
        const auto result = std::system(cmd.c_str());
        std::cout << "- Successful: " << (result == 0 ? "Yes" : "No") << std::endl;
        if (result != 0) {
            return err::AcbsErr { .type = err::AcbsErrType::CompileCommand, .extraInfo = "" };
        }
    }
    std::cout << std::endl;

    if (!std::filesystem::exists(proj.project.name) || srcObjZip.size() > 0) {
        std::cout << "Linking: " << std::endl;
        const auto linkCmd = linkObjsCmd(proj);
        std::cout << "- Command: '" << linkCmd << "'" << std::endl;
        const auto linkRes = std::system(linkCmd.c_str());
        std::cout << "- Successful: " << (linkRes == 0 ? "Yes" : "No") << std::endl;
        if (linkRes != 0) {
            return err::AcbsErr { .type = err::AcbsErrType::LinkCommand, .extraInfo = "" };
        }
    } else {
        std::cout << "Nothing to do." << std::endl;
    }

    return std::nullopt;
}

void acbs::build::clean(const ini::Project &proj) {
    try {
        std::cout << "Removing " << proj.project.build << std::endl;
        std::filesystem::remove_all(proj.project.build);
    } catch(...) {}
    try {
        std::cout << "Removing " << proj.project.name << std::endl;
        std::filesystem::remove_all(proj.project.name);
    } catch(...) {}
}

static std::vector<std::pair<std::filesystem::path, std::filesystem::path>> getDeltaFiles(
        const ini::Project &proj) {
    // Get all C files first
    std::vector<std::filesystem::path> sourceFiles;
    for (const auto &fldr : proj.project.source) {
        for (const auto &entry : std::filesystem::directory_iterator(fldr)) {
            if (entry.is_regular_file()
                    && (
                        (proj.project.isCpp && entry.path().extension() == ".cpp")
                            || (!proj.project.isCpp && entry.path().extension() == ".c")
                    )) {
                sourceFiles.push_back(entry.path());
            }
        }
    }

    // Figure out which files need to be rebuilt
    // If a .c/.cpp file is newer than its .o file OR that .o files doesn't exist, then rebuild
    std::vector<std::pair<std::filesystem::path, std::filesystem::path>> sourceObjZip;
    for (const auto &sourceFile : sourceFiles) {
        const auto objectName = proj.project.build + "/" + sourceFile.stem().string() + ".o";

        if (!std::filesystem::exists(objectName)) {
            sourceObjZip.push_back(std::make_pair(sourceFile, objectName));
            continue;
        }

        const auto sourceWriteTime = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                std::filesystem::last_write_time(sourceFile)
                    - std::filesystem::file_time_type::clock::now()
                    + std::chrono::system_clock::now()
            ).time_since_epoch()
        ).count();
        const auto objWriteTime = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                std::filesystem::last_write_time(objectName)
                    - std::filesystem::file_time_type::clock::now()
                    + std::chrono::system_clock::now()
            ).time_since_epoch()
        ).count();
        if (sourceWriteTime > objWriteTime) {
            sourceObjZip.push_back(std::make_pair(sourceFile, objectName));
            continue;
        }
    }

    return sourceObjZip;
}

static std::string buildFileCmd(
        const ini::Project &proj, const bool isDebug,
        const std::filesystem::path &src, const std::filesystem::path &obj) {
    std::stringstream cmd;
    cmd
        << "mkdir -p " << proj.project.build << "&& "
        << proj.compiler.compiler << " "
        << (isDebug ? proj.compiler.debugFlags : proj.compiler.flags) << " "
        << proj.compiler.objoutpfx << obj << " ";
    for (const auto &incPath : proj.project.include) {
        cmd << proj.compiler.incpfx << incPath << " ";
    }
    cmd << proj.compiler.srcpfx << src;
    return cmd.str();
}

static std::string linkObjsCmd(const ini::Project &proj) {
    // Get all .o files first
    std::vector<std::filesystem::path> objs;
    for (const auto &entry : std::filesystem::directory_iterator(proj.project.build)) {
        if (entry.is_regular_file() && entry.path().extension() == ".o") {
            objs.push_back(entry.path());
        }
    }

    std::stringstream cmd;
    cmd << proj.compiler.linker << " ";
    if (proj.compiler.linkerflagsafter) {
        cmd << proj.compiler.linkeroutpfx << proj.project.name << " ";
        for (const auto &obj : objs) {
            cmd << obj << " ";
        }
        cmd << proj.compiler.linkerFlags;
    } else {
        cmd << proj.compiler.linkerFlags << " ";
        cmd << proj.compiler.linkeroutpfx << proj.project.name << " ";
        for (const auto &obj : objs) {
            cmd << obj << " ";
        }
    }
    return cmd.str();
}

